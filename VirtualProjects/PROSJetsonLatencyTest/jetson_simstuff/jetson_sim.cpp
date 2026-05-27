// jetson_sim.cpp
// Compile: g++ -std=c++17 -pthread -o jetson_sim jetson_sim.cpp
// Run:     ./jetson_sim /dev/cu.usbmodem11101

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <thread>
#include <atomic>
#include <cstring>
#include <cstdint>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>

using steady_clk = std::chrono::steady_clock;
using us_t       = std::chrono::microseconds;
using ms_t       = std::chrono::milliseconds;

static long long now_us() {
    return std::chrono::duration_cast<us_t>(
        steady_clk::now().time_since_epoch()).count();
}

// ─── Open serial in background thread to avoid blocking main ─────────────────
static int open_serial(const char* port) {
    std::atomic<int> fd{-2}; // -2 = not done yet

    std::thread opener([&]() {
        // O_NONBLOCK on cu.* devices should not block,
        // but we do it in a thread just in case
        int f = ::open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
        if (f >= 0) {
            struct termios tty{};
            tcgetattr(f, &tty);
            cfsetospeed(&tty, B115200);
            cfsetispeed(&tty, B115200);
            tty.c_cflag  = (tty.c_cflag & ~CSIZE) | CS8;
            tty.c_cflag &= ~(PARENB | CSTOPB | CRTSCTS);
            tty.c_cflag |= CLOCAL | CREAD;
            tty.c_iflag  = IGNBRK;
            tty.c_lflag  = 0;
            tty.c_oflag  = 0;
            tty.c_cc[VMIN]  = 0;
            tty.c_cc[VTIME] = 5; // 0.5s read timeout
            tcsetattr(f, TCSANOW, &tty);
            tcflush(f, TCIOFLUSH);
        }
        fd.store(f);
    });

    // Wait up to 5 seconds for open to complete
    auto deadline = steady_clk::now() + std::chrono::seconds(5);
    while (fd.load() == -2 && steady_clk::now() < deadline) {
        std::this_thread::sleep_for(ms_t(100));
        std::cout << "." << std::flush;
    }
    std::cout << "\n";

    if (opener.joinable()) opener.detach();

    int result = fd.load();
    if (result == -2) {
        std::cerr << "[JETSON] open() timed out after 5s\n";
        return -1;
    }
    return result;
}

// ─── Read with select() timeout ───────────────────────────────────────────────
static ssize_t timed_read(int fd, void* buf, size_t n, int timeout_ms) {
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    struct timeval tv;
    tv.tv_sec  = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;
    int ret = select(fd + 1, &rfds, nullptr, nullptr, &tv);
    if (ret <= 0) return 0;
    return ::read(fd, buf, n);
}

// ─── Read exactly N bytes with timeout ───────────────────────────────────────
static bool read_exact(int fd, void* buf, size_t n, int timeout_ms = 2000) {
    size_t got = 0;
    auto deadline = steady_clk::now() + ms_t(timeout_ms);
    while (got < n && steady_clk::now() < deadline) {
        ssize_t r = timed_read(fd, (uint8_t*)buf + got, n - got, 100);
        if (r > 0) got += r;
    }
    return got == n;
}

// ─── Build PROS stdin packet: AA 55 20 <len_lo> <len_hi> <data> ──────────────
static std::vector<uint8_t> make_stdin_packet(const std::string& text) {
    std::vector<uint8_t> pkt;
    pkt.push_back(0xAA);
    pkt.push_back(0x55);
    pkt.push_back(0x20);
    uint16_t len = (uint16_t)text.size();
    pkt.push_back((uint8_t)(len & 0xFF));
    pkt.push_back((uint8_t)(len >> 8));
    for (char c : text) pkt.push_back((uint8_t)c);
    return pkt;
}

// ─── Read PROS packets until we find stdout text containing PONG ──────────────
static std::string read_pros_stdout(int fd, int timeout_ms = 2000) {
    auto deadline = steady_clk::now() + ms_t(timeout_ms);
    uint8_t b[2] = {0, 0};

    while (steady_clk::now() < deadline) {
        uint8_t c;
        ssize_t n = timed_read(fd, &c, 1, 100);
        if (n <= 0) continue;

        b[0] = b[1]; b[1] = c;
        if (b[0] != 0xAA || b[1] != 0x55) continue;

        // Got header — read cmd + 2 byte length
        uint8_t cmd;
        uint16_t len;
        if (!read_exact(fd, &cmd, 1, 200)) continue;
        if (!read_exact(fd, &len, 2, 200)) continue;

        if (len == 0 || len > 512) continue;

        std::vector<uint8_t> data(len);
        if (!read_exact(fd, data.data(), len, 500)) continue;

        // 0x21 = stdout from user program
        if (cmd == 0x21) {
            std::string text(data.begin(), data.end());
            if (text.find("PONG:") != std::string::npos) {
                // trim whitespace
                while (!text.empty() &&
                       (text.back() == '\n' || text.back() == '\r'))
                    text.pop_back();
                return text;
            }
        }
    }
    return "";
}

int main(int argc, char* argv[]) {
    const char* port = argc > 1 ? argv[1] : "/dev/cu.usbmodem11101";

    std::cout << "[JETSON] Opening " << port << " ";
    int fd = open_serial(port);
    if (fd < 0) {
        std::cerr << "[JETSON] Failed to open port\n";
        return 1;
    }
    std::cout << "[JETSON] Connected\n\n";

    // Give V5 a moment to settle
    std::this_thread::sleep_for(ms_t(500));

    const int ITERATIONS = 100;
    std::vector<long long> rtts;
    rtts.reserve(ITERATIONS);
    std::ofstream logfile("latency_log.csv");
    logfile << "frame,send_us,recv_us,rtt_us\n";

    for (int i = 0; i < ITERATIONS; i++) {
        std::string ping = "PING:" + std::to_string(i) + "\n";
        auto pkt = make_stdin_packet(ping);

        long long t_send = now_us();
        ::write(fd, pkt.data(), pkt.size());

        std::string response = read_pros_stdout(fd, 2000);
        long long t_recv = now_us();

        if (response.empty()) {
            std::cout << "[JETSON] Timeout frame " << i << "\n";
            continue;
        }

        long long rtt = t_recv - t_send;
        rtts.push_back(rtt);
        logfile << i << "," << t_send << ","
                << t_recv << "," << rtt << "\n";

        std::cout << "[JETSON] frame=" << i
                  << "  RTT=" << rtt << " us"
                  << "  (" << rtt/1000.0 << " ms)"
                  << "  resp='" << response << "'\n";

        std::this_thread::sleep_for(ms_t(33));
    }

    logfile.close();
    ::close(fd);

    if (!rtts.empty()) {
        long long sum  = std::accumulate(rtts.begin(), rtts.end(), 0LL);
        long long mean = sum / (long long)rtts.size();
        long long mn   = *std::min_element(rtts.begin(), rtts.end());
        long long mx   = *std::max_element(rtts.begin(), rtts.end());
        std::cout << "\n=== Latency Summary ===\n"
                  << "Samples : " << rtts.size() << "\n"
                  << "Mean RTT: " << mean << " us ("
                  << mean/1000.0 << " ms)\n"
                  << "Min  RTT: " << mn << " us\n"
                  << "Max  RTT: " << mx << " us\n"
                  << "Log saved to latency_log.csv\n";
    }

    return 0;
}
// jetson_diag.cpp — sends one PING and prints every raw byte received
// Compile: g++ -std=c++17 -pthread -o jetson_diag jetson_diag.cpp
// Run:     ./jetson_diag /dev/cu.usbmodem11101

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <atomic>
#include <cstdint>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <sys/select.h>

using steady_clk = std::chrono::steady_clock;
using ms_t       = std::chrono::milliseconds;

static int open_serial(const char* port) {
    std::atomic<int> fd{-2};
    std::thread opener([&]() {
        int f = ::open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
        if (f >= 0) {
            struct termios tty{};
            tcgetattr(f, &tty);
            cfsetospeed(&tty, B115200);
            cfsetispeed(&tty, B115200);
            tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
            tty.c_cflag &= ~(PARENB | CSTOPB | CRTSCTS);
            tty.c_cflag |= CLOCAL | CREAD;
            tty.c_iflag = IGNBRK;
            tty.c_lflag = 0;
            tty.c_oflag = 0;
            tty.c_cc[VMIN]  = 0;
            tty.c_cc[VTIME] = 5;
            tcsetattr(f, TCSANOW, &tty);
            tcflush(f, TCIOFLUSH);
        }
        fd.store(f);
    });
    auto deadline = steady_clk::now() + std::chrono::seconds(5);
    while (fd.load() == -2 && steady_clk::now() < deadline)
        std::this_thread::sleep_for(ms_t(100));
    if (opener.joinable()) opener.detach();
    return fd.load() == -2 ? -1 : fd.load();
}

static ssize_t timed_read(int fd, void* buf, size_t n, int ms) {
    fd_set rfds; FD_ZERO(&rfds); FD_SET(fd, &rfds);
    struct timeval tv{ ms/1000, (ms%1000)*1000 };
    return select(fd+1, &rfds, nullptr, nullptr, &tv) > 0
           ? ::read(fd, buf, n) : 0;
}

int main(int argc, char* argv[]) {
    const char* port = argc > 1 ? argv[1] : "/dev/cu.usbmodem11101";
    std::cout << "[DIAG] Opening " << port << "...\n";
    int fd = open_serial(port);
    if (fd < 0) { std::cerr << "Failed\n"; return 1; }
    std::cout << "[DIAG] Connected\n";

    // Wait for V5 to settle then flush anything already in the buffer
    std::this_thread::sleep_for(ms_t(500));
    tcflush(fd, TCIOFLUSH);

    // Send a PROS stdin packet with "PING:0\n"
    const char* text = "PING:0\n";
    uint16_t len = (uint16_t)strlen(text);
    uint8_t pkt[256];
    pkt[0] = 0xAA; pkt[1] = 0x55; pkt[2] = 0x20;
    pkt[3] = len & 0xFF; pkt[4] = len >> 8;
    memcpy(pkt + 5, text, len);
    size_t pkt_len = 5 + len;

    std::cout << "[DIAG] Sending PING packet ("
              << pkt_len << " bytes): ";
    for (size_t i = 0; i < pkt_len; i++)
        std::cout << std::hex << std::setw(2)
                  << std::setfill('0') << (int)pkt[i] << " ";
    std::cout << std::dec << "\n\n";

    ::write(fd, pkt, pkt_len);

    // Print every byte received for 3 seconds
    std::cout << "[DIAG] Raw bytes received for 3 seconds:\n";
    auto deadline = steady_clk::now() + std::chrono::seconds(3);
    int count = 0;
    while (steady_clk::now() < deadline) {
        uint8_t c;
        ssize_t n = timed_read(fd, &c, 1, 100);
        if (n <= 0) continue;
        std::cout << std::hex << std::setw(2)
                  << std::setfill('0') << (int)c << " ";
        if (c >= 32 && c < 127)
            std::cout << "(" << (char)c << ") ";
        if (++count % 8 == 0) std::cout << "\n";
        std::cout << std::flush;
    }
    std::cout << std::dec << "\n\n[DIAG] Done. Total bytes: " << count << "\n";

    ::close(fd);
    return 0;
}
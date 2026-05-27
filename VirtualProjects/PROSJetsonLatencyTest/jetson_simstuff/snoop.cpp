// snoop.cpp — dumps raw bytes from the serial port so we can see what V5 is sending
// Compile: g++ -std=c++17 -o snoop snoop.cpp
// Run:     ./snoop /dev/tty.usbmodem11103

#include <iostream>
#include <iomanip>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <cstdint>

int main(int argc, char* argv[]) {
    const char* port = argc > 1 ? argv[1] : "/dev/tty.usbmodem11103";

    int fd = ::open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0) { std::cerr << "Cannot open " << port << "\n"; return 1; }

    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);

    struct termios tty{};
    tcgetattr(fd, &tty);
    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);
    tty.c_cflag  = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_cflag &= ~(PARENB | CSTOPB | CRTSCTS);
    tty.c_cflag |= CLOCAL | CREAD;
    tty.c_iflag  = IGNBRK;
    tty.c_lflag  = 0;
    tty.c_oflag  = 0;
    tty.c_cc[VMIN]  = 0;
    tty.c_cc[VTIME] = 20;
    tcsetattr(fd, TCSANOW, &tty);
    tcflush(fd, TCIOFLUSH);

    std::cout << "Listening on " << port << " — printing raw bytes:\n\n";

    uint8_t buf[256];
    int count = 0;
    while (count < 200) {
        ssize_t n = read(fd, buf, sizeof(buf));
        if (n <= 0) continue;
        for (ssize_t i = 0; i < n; i++) {
            // print hex
            std::cout << std::hex << std::setw(2)
                      << std::setfill('0')
                      << (int)buf[i] << " ";
            // print ASCII if printable
            char c = (char)buf[i];
            if (c >= 32 && c < 127)
                std::cout << "(" << c << ") ";
            else
                std::cout << "    ";

            if (++count % 8 == 0) std::cout << "\n";
        }
    }
    std::cout << "\n";
    ::close(fd);
    return 0;
}
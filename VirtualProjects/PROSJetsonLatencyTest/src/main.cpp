// main.cpp — PROS V5 kernel 4.x
// Uses text-based JSON over stdout — works with PROS COBS framing
// Jetson sends:  PING:<framecnt>\n
// V5 replies:    PONG:<framecnt>:<rx_ms>\n

#include "main.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

void initialize() {
    pros::lcd::initialize();
    pros::lcd::set_text(0, "Waiting for Jetson");
}

void opcontrol() {
    setvbuf(stdout, nullptr, _IONBF, 0);
    setvbuf(stdin,  nullptr, _IONBF, 0);

    uint32_t good = 0, timeouts = 0;
    char line[64];

    while (true) {
        // Read a line from Jetson e.g. "PING:42\n"
        if (fgets(line, sizeof(line), stdin)) {
            uint32_t rx_ms = pros::millis();

            // Parse frame number
            int framecnt = -1;
            if (strncmp(line, "PING:", 5) == 0) {
                framecnt = atoi(line + 5);
            }

            if (framecnt >= 0) {
                good++;
                // Reply with PONG:<framecnt>:<rx_ms>
                printf("PONG:%d:%lu\n", framecnt, rx_ms);
                fflush(stdout);

                if (good % 5 == 0) {
                    pros::lcd::set_text(0, "Jetson connected!");
                    pros::lcd::print(1, "OK      : %u", good);
                    pros::lcd::print(2, "Timeouts: %u", timeouts);
                    pros::lcd::print(3, "Frame   : %d", framecnt);
                }
            }
        } else {
            timeouts++;
            pros::lcd::print(4, "Timeouts: %u", timeouts);
        }

        pros::delay(1);
    }
}
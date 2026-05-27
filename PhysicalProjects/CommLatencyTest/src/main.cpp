#include "main.h"
#include <cstring>
#include <string.h>

#define IS_TRANSMITTER 0
#define TRANSMITTER_PORT 4
#define RECEIVER_PORT 1


#define LINK_PORT 1
#define LINK_ID "PING_LI"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
	
void opcontrol() {

#if IS_TRANSMITTER
    pros::Link tx_link(1, LINK_ID, pros::E_LINK_TX, true);
    pros::delay(2000);

    while (!tx_link.connected()) {
        pros::delay(20);
    }

    pros::lcd::print(2, "TX Connected");

	std::array<unsigned char, 1> ping_byte;
	ping_byte.fill(0);
    uint32_t response    = 0;
    uint32_t average_rtt = 0;

    for (int i = 0; i < 50; i++) {
        pros::lcd::print(0, "Iteration: %d", i);

        // flush any stale bytes
        while (tx_link.raw_receivable_size() > 0) {
            uint8_t dump[32];
            uint32_t avail = tx_link.raw_receivable_size();
            if (avail > sizeof(dump)) avail = sizeof(dump);
            tx_link.receive_raw(dump, avail);
        }

        uint32_t send_time = pros::millis();
        tx_link.transmit_raw(&ping_byte, sizeof(uint32_t));

        response = 0;
        bool got_echo = false;

        while (pros::millis() - send_time < 500) {
            int len = tx_link.receive_raw(&response, sizeof(response));
            if (len == sizeof(response)) {
                got_echo = true;
                break;
            }
            pros::delay(5);
        }

        uint32_t rtt = got_echo ? (pros::millis() - send_time) : 500;
        average_rtt += rtt;

        pros::lcd::print(1, "RTT: %d ms", rtt);
        printf("RTT: %d ms (resp=%u)\n", rtt, response);

        pros::delay(50);
    }

    pros::lcd::print(0, "Final RTT: %d ms", average_rtt / 50);
    pros::lcd::print(6, "Test complete");
    while (true) pros::delay(100);

#else   // RECEIVER

    pros::Link rx_link(1, LINK_ID, pros::E_LINK_RX, true);
    pros::delay(2000);

    while (!rx_link.connected()) {
        pros::delay(20);
    }

    pros::lcd::print(2, "RX Connected");

	std::array<unsigned char, 1> buf;

    while (true) {
        int len = rx_link.receive_raw(&buf, sizeof(buf));
        if (len == sizeof(buf)) {
            pros::lcd::print(4, "Received: %u", buf);
            // echo back exactly what we got
            rx_link.transmit_raw(&buf, sizeof(uint32_t));
        }
        pros::delay(5);
    }

#endif
}
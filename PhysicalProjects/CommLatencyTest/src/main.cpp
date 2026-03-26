#include "main.h"
#include <cstring>
#include <string.h>

#define IS_TRANSMITTER 1
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
	pros::Link tx_link(LINK_PORT, "LINK_ID", pros::E_LINK_TRANSMITTER);
	pros::Task::delay(2000);

	char* message = "check";
	char* expected = "check";
	char result[32];

	tx_link.transmit((void*)message, strlen(message) + 1);
	tx_link.receive((void*)result, strlen(expected) + 1);
	while(strcmp(result, expected) != 0)
	{
		pros::lcd::print(2, "Instead Received: %d", pros::millis());
		tx_link.transmit((void*)message, strlen(message) + 1);
		pros::delay(10);
		tx_link.receive((void*)result, strlen(expected) + 1);
		pros::delay(10);
	}

	pros::lcd::print(2, "Received: check");

	uint8_t ping_byte = 4;
	uint8_t response = 0;
	uint32_t average_rtt = 0;

	for(int i = 0; i < 50; i++) {
		uint32_t send_time = pros::millis();

		tx_link.transmit((void*)&ping_byte, sizeof(ping_byte));

		while (true) {
			int len = tx_link.receive(&response, 1);

			if (len > 0 && response == ping_byte) {
				break;
			}

			pros::delay(5);
		}

		uint32_t rtt = pros::millis() - send_time;

		average_rtt += rtt;
		pros::lcd::print(0, "RTT: %d ms", average_rtt);

		pros::Task::delay(50);

		//pros::Task::delay(500);
	}

	pros::lcd::print(0, "Final RTT: %d ms", average_rtt/50);


#endif

#if !IS_TRANSMITTER
	pros::Link rx_link(LINK_PORT, "LINK_ID", pros::E_LINK_RECIEVER);
	pros::Task::delay(2000);

	char* expected = "check";
	char result[32];

	rx_link.receive((void*)result, strlen(expected) + 1);
	while(strcmp(result, expected) != 0)
	{
		pros::lcd::print(2, "nah Instead Received: %s", result);
		rx_link.receive((void*)result, strlen(expected) + 1);
		pros::delay(10);
	}

	rx_link.transmit(expected, strlen(expected) + 1);
	pros::lcd::print(2, "Transmitted: check");

	uint8_t buf = 0;
	uint8_t ping_byte = 4;

	while (true) {
		while (true) {
			int len = rx_link.receive(&buf, 1);

			if (len > 0 && buf == ping_byte) {
				break;
			}

			pros::delay(5);
		}

		rx_link.transmit((void*)&buf, 1);

		// switch back
		pros::Task::delay(50);
	}
#endif


#pragma region ARIANA

	// pros::Controller master(pros::E_CONTROLLER_MASTER);

	// if(IS_TRANSMITTER)
	// {
	// 	pros::Link receiver(TRANSMITTER_PORT, "LINK_ID", pros::E_LINK_RECIEVER);
	// 	pros::delay(2000);
	// 	char* expected = "Check";
	// 	char result[32];

	// 	int expectedLength = strlen(expected) + 1;
	// 	receiver.receive((void*)result, expectedLength);	
	// 	while(strcmp(result, expected) != 0)
	// 	{
	// 		pros::lcd::print(2, "Instead Received: %s", result);
	// 		receiver.receive((void*)result, expectedLength);
	// 		pros::delay(10);
	// 	}
	// 	pros::lcd::print(2, "Received: Check");

	// 	pros::Link transmitter(TRANSMITTER_PORT, "LINK_ID", pros::E_LINK_TRANSMITTER);
	// 	char* message = "superimportantmessage";
	// 	transmitter.transmit_raw(message, strlen(message) + 1);
	// 	double startTime = pros::millis();

	// 	expected = "recieved";
	// 	char secondResult[32];

	// 	//pros::Link receiver2(TRANSMITTER_PORT, "LINK_ID", pros::E_LINK_RECIEVER);
		
	// 	expectedLength = strlen(expected) + 1;
	// 	transmitter.receive_raw((void*)secondResult, expectedLength);
	// 	while(strcmp(secondResult, expected) != 0)
	// 	{
	// 		pros::lcd::print(2, "Instead Received: %s", secondResult);
	// 		pros::lcd::print(3, "Current: %f ms", pros::millis() - startTime);
	// 		transmitter.receive_raw((void*)secondResult, expectedLength);
	// 		pros::delay(10);
	// 	}

	// 	pros::lcd::print(4, "Latency: %f ms", pros::millis() - startTime);
	// }
	// else
	// {
	// 	pros::Link transmitter(RECEIVER_PORT, "LINK_ID", pros::E_LINK_TRANSMITTER);
	// 	pros::Link receiver(RECEIVER_PORT, "LINK_ID", pros::E_LINK_RECIEVER);
	// 	pros::delay(2000);
	// 	char* message = "Check";
	// 	char* expected = "superimportantmessage";
	// 	char result[32];

	// 	int messageLength = strlen(message) + 1;
	// 	int expectedLength = strlen(expected) + 1;
	// 	receiver.receive_raw((void*)result, expectedLength);
	// 	while(strcmp(result, expected) != 0)
	// 	{
	// 		pros::lcd::print(2, "Not Received: superimportantmessage");
	// 		receiver.receive_raw((void*)result, expectedLength);

	// 		transmitter.transmit_raw(message, messageLength);

	// 		pros::delay(10);
	// 	}
		

	// 	//pros::Link transmitter2(RECEIVER_PORT, "LINK_ID", pros::E_LINK_TRANSMITTER);

	// 	message = "recieved";
	// 	messageLength = strlen(message) + 1;
	// 	while(true)
	// 	{
	// 		receiver.transmit_raw(message, messageLength);
	// 		pros::lcd::print(2, "Transmitted: recieved");
	// 		pros::delay(10);
	// 	}
	// }
#pragma endregion
}
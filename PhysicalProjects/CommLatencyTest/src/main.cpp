#include "main.h"
#include <cstring>

#define IS_TRANSMITTER 0
#define TRANSMITTER_PORT 4
#define RECEIVER_PORT 1

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
	pros::Controller master(pros::E_CONTROLLER_MASTER);

	if(IS_TRANSMITTER)
	{
		pros::Link receiver(TRANSMITTER_PORT, "LINK_ID", pros::E_LINK_RECIEVER);
		pros::delay(2000);
		char* expected = "Check";
		char result[32];

		int expectedLength = strlen(expected) + 1;
		receiver.receive((void*)result, expectedLength);	
		while(strcmp(result, expected) != 0)
		{
			pros::lcd::print(2, "Instead Received: %s", result);
			receiver.receive((void*)result, expectedLength);
			pros::delay(10);
		}
		pros::lcd::print(2, "Received: Check");

		pros::Link transmitter(TRANSMITTER_PORT, "LINK_ID", pros::E_LINK_TRANSMITTER);
		char* message = "superimportantmessage";
		transmitter.transmit_raw(message, strlen(message) + 1);
		double startTime = pros::millis();

		expected = "recieved";
		char secondResult[32];

		//pros::Link receiver2(TRANSMITTER_PORT, "LINK_ID", pros::E_LINK_RECIEVER);
		
		expectedLength = strlen(expected) + 1;
		transmitter.receive_raw((void*)secondResult, expectedLength);
		while(strcmp(secondResult, expected) != 0)
		{
			pros::lcd::print(2, "Instead Received: %s", secondResult);
			pros::lcd::print(3, "Current: %f ms", pros::millis() - startTime);
			transmitter.receive_raw((void*)secondResult, expectedLength);
			pros::delay(10);
		}

		pros::lcd::print(4, "Latency: %f ms", pros::millis() - startTime);
	}
	else
	{
		pros::Link transmitter(RECEIVER_PORT, "LINK_ID", pros::E_LINK_TRANSMITTER);
		pros::Link receiver(RECEIVER_PORT, "LINK_ID", pros::E_LINK_RECIEVER);
		pros::delay(2000);
		char* message = "Check";
		char* expected = "superimportantmessage";
		char result[32];

		int messageLength = strlen(message) + 1;
		int expectedLength = strlen(expected) + 1;
		receiver.receive_raw((void*)result, expectedLength);
		while(strcmp(result, expected) != 0)
		{
			pros::lcd::print(2, "Not Received: superimportantmessage");
			receiver.receive_raw((void*)result, expectedLength);

			transmitter.transmit_raw(message, messageLength);

			pros::delay(10);
		}
		

		//pros::Link transmitter2(RECEIVER_PORT, "LINK_ID", pros::E_LINK_TRANSMITTER);

		message = "recieved";
		messageLength = strlen(message) + 1;
		while(true)
		{
			receiver.transmit_raw(message, messageLength);
			pros::lcd::print(2, "Transmitted: recieved");
			pros::delay(10);
		}
	}
}
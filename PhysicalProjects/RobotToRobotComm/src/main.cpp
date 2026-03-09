#include "main.h"
#include <cstring>

#define IS_TRANSMITTER 1
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
	pros::Link masterTransmitter(TRANSMITTER_PORT, "LINK_ID", pros::E_LINK_TRANSMITTER);
	pros::Link masterReceiver(TRANSMITTER_PORT, "LINK_ID", pros::E_LINK_RECIEVER);
	pros::Link slaveReceiver(RECEIVER_PORT, "LINK_ID", pros::E_LINK_RECIEVER);
	pros::Link slaveTransmitter(RECEIVER_PORT, "LINK_ID", pros::E_LINK_RECIEVER);

	pros::MotorGroup left_transmitter({10, 9});
	pros::MotorGroup right_transmitter({-20, -19});

	pros::MotorGroup left_receiver({16, 20});
	pros::MotorGroup right_receiver({-15, -11});

	left_transmitter.tare_position();
	right_transmitter.tare_position();

	left_receiver.tare_position();
	right_receiver.tare_position();

	if(IS_TRANSMITTER)
	{	
		char* expected = "Check";
		char result[32];
		masterReceiver.receive(result, sizeof(result));		

		while(strcmp(result, expected) != 0)
		{
			pros::lcd::print(2, "Not Received: Check");
			masterReceiver.receive((void*)result, strlen(expected) + 1);
		}

		left_transmitter.move(20);
		right_transmitter.move(20);

		while(abs(left_transmitter.get_position(0)) < 500){}
	}


	while(true)
	{
		if(IS_TRANSMITTER)
		{
			char* data = "Move";

			masterTransmitter.transmit(data, strlen(data) + 1);
			pros::lcd::print(2, "Transmitted: Move");
			left_transmitter.move(0);
			right_transmitter.move(0);
		}
		else
		{
			char* expected = "Move";
			char result[32];
			slaveReceiver.receive(result, sizeof(result));		

			slaveReceiver.receive((void*)result, strlen(expected) + 1);

			if(strcmp(result, expected) == 0)
			{
				pros::lcd::print(2, "Recieved: Move");
				left_receiver.move(20);
				right_receiver.move(20);
			}
			else
			{
				pros::lcd::print(2, "Instead recieved: %s", result);

				char* data = "Check";
				slaveTransmitter.transmit(data, strlen(result) + 1);
			}
		}
		pros::delay(10);
	}
}
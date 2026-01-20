/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       arianadadgostar                                           */
/*    Created:      1/17/2026, 6:42:03 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
#define DIAMETER 4
#define ROBOTLENGTH 10

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;
// vex::motor       TopLeft( vex::PORT16, vex::gearSetting::ratio18_1, true );
// vex::motor       TopRight( vex::PORT15, vex::gearSetting::ratio18_1, false );
// vex::motor       BottomLeft( vex::PORT20, vex::gearSetting::ratio18_1, false );
// vex::motor       BottomRight( vex::PORT11, vex::gearSetting::ratio18_1, true );
vex::motor       TopLeft( vex::PORT19, vex::gearSetting::ratio18_1, false );
vex::motor       TopRight( vex::PORT9, vex::gearSetting::ratio18_1, true );
vex::motor       BottomLeft( vex::PORT20, vex::gearSetting::ratio18_1, false );
vex::motor       BottomRight( vex::PORT10, vex::gearSetting::ratio18_1, true );


vex::controller Controller;

/*
AL = (pi)r * (theta/180)

dL - dR = (R + L)chTheta - (R - L)chTheta;
*/

float vertical = 0;
float horizontal = 0;
float theta = 0;

// define your global instances of motors and other devices here

int powerMap[127] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                  , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                  , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                  , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                  , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                  , 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0
                  , 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
                  , 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
                  , 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
                  , 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
                  , 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
                  , 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
                  , 100, 100, 100, 100, 100, 100, 100};

void RunMotor(int velocity, vex::motor motor)
{
    motor.spin(vex::directionType::fwd, velocity, vex::velocityUnits::pct);
}

void RunChassis()
{
    int LeftVal = powerMap[Controller.Axis3.position()] + powerMap[Controller.Axis1.position()];
    int RightVal = powerMap[Controller.Axis3.position()] - powerMap[Controller.Axis1.position()];
    
    RunMotor(LeftVal/5, TopLeft);
    RunMotor(RightVal/5, TopRight);
    RunMotor(LeftVal/5, BottomLeft);
    RunMotor(RightVal/5, BottomRight);
}

void CalculateMovement()
{
    int dL = TopLeft.current(vex::percent);
    int dR = TopRight.current(vex::percent);
    int hypotenuse = (dL + dR) / 2;
    int thetaChange = (dL - dR) / (2*ROBOTLENGTH);

    vertical += hypotenuse * cos(theta + thetaChange/2);
    horizontal += hypotenuse * sin(theta + thetaChange/2);
    theta += thetaChange;

    TopLeft.resetPosition();
    TopRight.resetPosition();
    BottomLeft.resetPosition();
    BottomRight.resetPosition();
}

void CalculateReturn()
{
    float returnVal = sqrt( (vertical * vertical) + (horizontal * horizontal) );
    int returnAngle = atan2(horizontal, vertical) - theta;

    printf("horizontal: %f\n", horizontal);
    printf("vertical: %f\n", vertical);
    printf("return val: %f\n", returnVal);

    while(abs(TopLeft.position(vex::deg)) <= returnAngle && abs(TopRight.position(vex::deg)) <= returnAngle)
    {
        RunMotor( returnVal * cos(returnAngle), TopLeft );
        RunMotor( returnVal * cos(returnAngle), BottomLeft );
        RunMotor( returnVal * sin(returnAngle), TopRight );
        RunMotor( returnVal * sin(returnAngle), BottomRight );
    }

    while(abs(TopLeft.position(vex::deg)) <= returnVal && abs(TopRight.position(vex::deg)) <= returnVal)
    {
        RunMotor( 50, TopLeft );
        RunMotor( 50, BottomLeft );
        RunMotor( 50, TopRight );
        RunMotor( 50, BottomRight );
    }
}

int main() {

    printf( "Hello V5" );
   
    while(1) {
        RunChassis();
        CalculateMovement();

        if(Controller.ButtonA.pressing())
        {
            thread bg(CalculateReturn);
        }
        // Allow other tasks to run
        this_thread::sleep_for(10);
    }
}

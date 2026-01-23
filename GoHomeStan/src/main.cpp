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
    int LeftVal = powerMap[Controller.Axis3.position()]; //+ powerMap[Controller.Axis1.position()];
    int RightVal = powerMap[Controller.Axis2.position()]; //- powerMap[Controller.Axis1.position()];
    
    RunMotor(LeftVal/5, TopLeft);
    RunMotor(RightVal/5, TopRight);
    RunMotor(LeftVal/5, BottomLeft);
    RunMotor(RightVal/5, BottomRight);
}

void CalculateMovement()
{
    float dL = TopLeft.current(vex::percent);
    float dR = TopRight.current(vex::percent);
    float hypotenuse = (dL + dR) / 2;
    float thetaChange = (dL - dR) / (2*ROBOTLENGTH);

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
    float returnAngle = atan2(horizontal, vertical) - theta;

    printf("horizontal: %f\n", horizontal);
    printf("vertical: %f\n", vertical);
    printf("return val: %f\n", returnVal);
    printf("return angle: %f\n", returnAngle);
    printf("theta: %f\n", theta);

    while(abs(TopLeft.position(vex::deg)) <= returnVal * cos(returnAngle) && abs(TopRight.position(vex::deg)) <= returnVal * sin(returnAngle))
    {
        RunMotor( returnVal * cos(returnAngle)*10, TopLeft );
        RunMotor( (returnVal * cos(returnAngle))*10, BottomLeft );
        RunMotor( returnVal * sin(returnAngle)*10, TopRight );
        RunMotor( returnVal * sin(returnAngle)*10, BottomRight );
    }

    while(abs(TopLeft.position(vex::deg)) <= returnVal && abs(TopRight.position(vex::deg)) <= returnVal)
    {
        RunMotor( -20, TopLeft );
        RunMotor( -20, BottomLeft );
        RunMotor( -20, TopRight );
        RunMotor( -20, BottomRight );
    }

    RunMotor(0, TopLeft);
    RunMotor(0, BottomLeft);
    RunMotor(0, TopRight);
    RunMotor(0, BottomRight);
}

int main() {

    TopLeft.resetPosition();
    TopRight.resetPosition();
    BottomLeft.resetPosition();
    BottomRight.resetPosition();
    printf( "Hello V5" );
   
    while(1) {
        RunChassis();
        CalculateMovement();

        if(Controller.ButtonA.pressing())
        {
            thread bg(CalculateReturn);
            break;
        }
        // Allow other tasks to run
        this_thread::sleep_for(10);
    }

    while(true)
    {
        CalculateReturn();
        this_thread::sleep_for(10);
    }
}

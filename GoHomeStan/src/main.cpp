/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       arianadadgostar                                           */
/*    Created:      1/17/2026, 6:42:03 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
#define DIAMETER 10

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;
vex::motor       TopLeft( vex::PORT16, vex::gearSetting::ratio18_1, true );
vex::motor       TopRight( vex::PORT15, vex::gearSetting::ratio18_1, false );
vex::motor       BottomLeft( vex::PORT20, vex::gearSetting::ratio18_1, false );
vex::motor       BottomRight( vex::PORT11, vex::gearSetting::ratio18_1, true );

vex::controller Controller;

/*
AL = (pi)d * (theta/180)

dL - dR = (R + L)chTheta - (R - L)chTheta;
*/

int vertical = 0;
int horizontal = 0;
int theta = 0;

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
    
    RunMotor(LeftVal, TopLeft);
    RunMotor(RightVal, TopRight);
    RunMotor(LeftVal, BottomLeft);
    RunMotor(RightVal, BottomRight);
}



void CalculateReturn()
{
    int returnVal = sqrt( (vertical * vertical) + (horizontal * horizontal) );
}

int main() {

    printf( "Hello V5" );
   
    while(1) {
        RunChassis();

        if(Controller.ButtonA.pressing())
        {

        }
        // Allow other tasks to run
        this_thread::sleep_for(10);
    }
}

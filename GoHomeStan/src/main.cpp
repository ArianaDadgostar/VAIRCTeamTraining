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
*/

enum class drivePosition {
    POSVERT,
    POSHORIZ,
    NEGVERT,
    NEGHORIZ
};

int vertical = 0;
int horizontal = 0;

drivePosition currentPosition = drivePosition::POSVERT;

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

void RunChassisUpdated()
{
    if(Controller.ButtonX.pressing())
}

void AddToPosition(int theta)
{
    switch(currentPosition)
    {
        case drivePosition::POSVERT:
        {
            if(TopLeft.velocity(vex::percent) < 0 && theta >= 90)
            {
                currentPosition = drivePosition::NEGHORIZ;
            }
            else if(theta >= 90)
            {
                currentPosition = drivePosition::POSHORIZ;
            }
            else if(TopLeft.velocity(vex::percent) > 0 && TopRight.velocity(vex::percent) > 0)
            {
                vertical += TopLeft.velocity(vex::percent);
            }

            break;
        }

        case drivePosition::POSHORIZ:
        {
            if(TopLeft.velocity(vex::percent) < 0 && theta >= 90)
            {
                currentPosition = drivePosition::NEGVERT;
            }
            else if(theta >= 90)
            {
                currentPosition = drivePosition::POSVERT;
            }
            else if(TopLeft.velocity(vex::percent) > 0 && TopRight.velocity(vex::percent) > 0)
            {
                horizontal += TopLeft.velocity(vex::percent);
            }

            break;
        }

        case drivePosition::NEGVERT:
        {
            if(TopLeft.velocity(vex::percent) > 0 && theta >= 90)
            {
                currentPosition = drivePosition::POSHORIZ;
            }
            else if(theta >= 90)
            {
                currentPosition = drivePosition::NEGHORIZ;
            }
            else if(TopLeft.velocity(vex::percent) > 0 && TopRight.velocity(vex::percent) > 0)
            {
                vertical -= TopLeft.velocity(vex::percent);
            }
            vertical -= TopLeft.velocity(vex::percent);

            break;
        }

        case drivePosition::NEGHORIZ:
        {
            if(TopLeft.velocity(vex::percent) > 0 && theta >= 90)
            {
                currentPosition = drivePosition::POSVERT;
            }
            else if(theta >= 90)
            {
                currentPosition = drivePosition::NEGVERT;
            }
            horizontal -= TopLeft.velocity(vex::percent);

            break;
        }
    }
}

void CalculatePosition()
{
    if(TopLeft.velocity(vex::percent) > 0 && TopRight.velocity(vex::percent) > 0) return;
    int AL = TopLeft.velocity(vex::percent);
    int theta = AL / ( (3.14 * DIAMETER) / 180 );

    if(theta < 90)
    {
        AddToPosition(theta);
    }
}

void CalculateReturn()
{
    int returnVal = sqrt( (vertical * vertical) + (horizontal * horizontal) );
}

int main() {

    printf( "Hello V5" );
   
    while(1) {
        RunChassis();

        CalculatePosition();

        if(Controller.ButtonA.pressing())
        {

        }
        // Allow other tasks to run
        this_thread::sleep_for(10);
    }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       arianadadgostar                                           */
/*    Created:      1/17/2026, 6:42:03 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
#define RADIUS 2
#define ROBOTLENGTH 15
#define ROBOTRADIUS 6

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;
// vex::motor       TopLeft( vex::PORT16, vex::gearSetting::ratio18_1, true );
// vex::motor       TopRight( vex::PORT15, vex::gearSetting::ratio18_1, false );
// vex::motor       BottomLeft( vex::PORT20, vex::gearSetting::ratio18_1, true );
// vex::motor       BottomRight( vex::PORT11, vex::gearSetting::ratio18_1, false );
vex::motor       TopLeft( vex::PORT19, false );
vex::motor       TopRight( vex::PORT9, true );
vex::motor       BottomLeft( vex::PORT20, false );
vex::motor       BottomRight( vex::PORT10, true );


vex::controller Controller;

/*
AL = (pi)r * (theta/180)

dL - dR = (R + L)chTheta - (R - L)chTheta;
*/

float vertical = 0;
float horizontal = 0;
float theta = 0;
float returnTheta = 0;

float returnVal;
//float returnAngle;


float leftVal;
float rightVal;

float TopRightAccumulated = 0;

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

void ReturnAngleCalculatedNew()
{
    float dL = TopLeft.position(vex::deg);
    float dR = TopRight.position(vex::deg);
    float thetaChange = ((dL - dR) * 2 * RADIUS * 3.14159265358979323846) / 360;
    thetaChange /= (2 * ROBOTLENGTH * 3.14159265358979323846);
    thetaChange *= 360;

    theta += thetaChange;
    vertical += ROBOTLENGTH * sin(thetaChange);
    horizontal += ROBOTLENGTH * (1 - cos(thetaChange));

    if(ROBOTLENGTH * sin(thetaChange) != 0 || ROBOTLENGTH * (1 - cos(thetaChange)) != 0)
    {
        printf("vertical: %f\n", vertical);
        printf("horizontal: %f\n", horizontal);
        printf("theta: %f\n", theta);
    }

    TopLeft.resetPosition();
    TopRight.resetPosition();
    BottomLeft.resetPosition();
    BottomRight.resetPosition();
}

void RunChassis()
{
    int LeftVal = powerMap[Controller.Axis3.position()]; //+ powerMap[Controller.Axis1.position()];
    int RightVal = powerMap[Controller.Axis2.position()]; //- powerMap[Controller.Axis1.position()];
    
    RunMotor(Controller.Axis3.position()/5, TopLeft);
    RunMotor(Controller.Axis2.position()/5, TopRight);
    RunMotor(Controller.Axis3.position()/5, BottomLeft);
    RunMotor(Controller.Axis2.position()/5, BottomRight);
}

void EstablishReturnVal()
{
    if(theta < 180)
    {
        returnVal = 180 - theta;
    }
    else
    {
        returnVal = -1 * theta;
    }
}

bool ReturnedAngleUpdated()
{
    float dL = TopLeft.current(vex::percent);
    float dR = TopRight.current(vex::percent);
    float thetaChange = ((dL - dR) * 2 * RADIUS * 3.14159265358979323846) / 360;
    thetaChange /= (2 * ROBOTLENGTH * 3.14159265358979323846);
    thetaChange *= 360;
    returnTheta += thetaChange;

    printf("lala\n");

    if(abs(returnTheta) >= abs(returnVal)) return true;

    float absTheta = abs(theta);

    if(absTheta > 180)
    {
        RunMotor( -absTheta, TopLeft );
        RunMotor( -absTheta, BottomLeft );
        RunMotor( absTheta, TopRight );
        RunMotor( absTheta, BottomRight );
    }
    else
    {
        RunMotor( absTheta, TopLeft );
        RunMotor( absTheta, BottomLeft );
        RunMotor( -absTheta, TopRight );
        RunMotor( -absTheta, BottomRight );
    }

    return false;
}

bool MoveToOrigin()
{
    while(abs(TopLeft.position(vex::deg)) <= returnVal && abs(TopRight.position(vex::deg)) <= returnVal)
    {
        RunMotor( 20, TopLeft );
        RunMotor( 20, BottomLeft );
        RunMotor( 20, TopRight );
        RunMotor( 20, BottomRight );
    }
    return false;
}

int main() {

    TopLeft.resetPosition();
    TopRight.resetPosition();
    BottomLeft.resetPosition();
    BottomRight.resetPosition();
    printf( "Hello V5" );
   
    while(1) {
        RunChassis();
        ReturnAngleCalculatedNew();

            printf("nothing works \n");
        if(Controller.ButtonA.pressing())
        {
            printf("ew\n");
            break;
        }
        // Allow other tasks to run
        this_thread::sleep_for(10);
    }

    //CalculateReturn();

    TopLeft.resetPosition();
    TopRight.resetPosition();
    BottomLeft.resetPosition();
    BottomRight.resetPosition();

    while(!ReturnedAngleUpdated()) this_thread::sleep_for(10);

    printf("brother what\n");

    TopLeft.resetPosition();
    TopRight.resetPosition();
    BottomLeft.resetPosition();
    BottomRight.resetPosition();

    // while(true)
    // {
    //     if(MoveToOrigin())
    //     {
    //         break;
    //     }
    //     this_thread::sleep_for(10);
    // }


    RunMotor(0, TopLeft);
    RunMotor(0, BottomLeft);
    RunMotor(0, TopRight);
    RunMotor(0, BottomRight);
}

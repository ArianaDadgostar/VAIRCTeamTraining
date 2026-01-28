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

float returnVal;
float returnAngle;

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
    //float thetaChange = (DIAMETER/2) * (dL - dR) / (2*ROBOTLENGTH); NOT WORKING
    float thetaChange = ((RADIUS) * (dL - dR)) / ROBOTLENGTH;

    theta += thetaChange;
    vertical += hypotenuse * cos(theta);
    horizontal += hypotenuse * sin(theta);


    printf("vertical: %f\n", vertical);
    printf("horizontal: %f\n", horizontal);
    printf("theta: %f\n", theta);

    TopLeft.resetPosition();
    TopRight.resetPosition();
    BottomLeft.resetPosition();
    BottomRight.resetPosition();
    vex::this_thread::sleep_for(20);
}

bool CalculateReturn()
{
    returnVal = sqrt( (vertical * vertical) + (horizontal * horizontal) );
    returnAngle = atan2(horizontal, vertical) - theta;
    printf("LEFT: %f\n", TopLeft.position(vex::deg));
    printf("RIGHT: %f\n", TopRight.position(vex::deg));

    // printf("horizontal: %f\n", horizontal);
    // printf("vertical: %f\n", vertical);
    // printf("return val: %f\n", returnVal);
    // printf("return angle: %f\n", returnVal * cos(returnAngle)*10);
        float leftVal = (ROBOTLENGTH * returnAngle) / RADIUS;
        float rightVal = (ROBOTLENGTH * returnAngle * -1) / RADIUS;
        
    // printf("maybe its this %f\n", leftVal);

    while(abs(TopLeft.position(vex::deg)) <= abs(leftVal) && abs(TopRight.position(vex::deg)) <= abs(rightVal))
    {
    printf("LEFT: %f\n", TopLeft.position(vex::deg));
    printf("RIGHT: %f\n", TopRight.position(vex::deg));
    printf("LEFTVAL: %f\n", leftVal);
    printf("RIGHTAL: %f\n", rightVal);
        //printf("oml what is not working cuh\n");
        //leftVal = ((returnVal * cos(returnAngle))/abs(returnVal * cos(returnAngle))) * 120;
        leftVal = (ROBOTLENGTH * returnAngle) / RADIUS;
        RunMotor( leftVal, TopLeft );
        RunMotor( leftVal, BottomLeft );


        rightVal = (ROBOTLENGTH * returnAngle * -1) / RADIUS;
        RunMotor( rightVal, TopRight );
        RunMotor( rightVal, BottomRight );
    }

    //int rightVal = (ROBOTLENGTH * returnAngle) / RADIUS;

    // while (abs(TopRight.position(vex::deg)) <= abs(rightVal))
    // {
    //     //int rightVal = ((returnVal * sin(returnAngle))/abs(returnVal * sin(returnAngle))) * 120;

    // }

    if(abs(TopLeft.position(vex::deg)) > returnVal * cos(returnAngle) && abs(TopRight.position(vex::deg)) > returnVal * sin(returnAngle))
    {
        return true;
    }

    return false;

    // RunMotor(0, TopLeft);
    // RunMotor(0, BottomLeft);
    // RunMotor(0, TopRight);
    // RunMotor(0, BottomRight);
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
        CalculateMovement();

        if(Controller.ButtonA.pressing())
        {
            break;
        }
        // Allow other tasks to run
        this_thread::sleep_for(10);
    }

    while(true)
    {
        if(CalculateReturn())
        {
            break;
        }
        this_thread::sleep_for(10);
    }

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

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

float vertical = 0;
float horizontal = 0;
float theta = 0;
float returnTheta = 0;

float returnVal;
float straightReturn;

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
    float average = (dL + dR) / 2;
    float thetaChange = ((dL - dR) * 2 * RADIUS * 3.14159265358979323846) / 360;
    thetaChange /= (2 * ROBOTLENGTH * 3.14159265358979323846);
    thetaChange *= 360;

    theta += thetaChange;
    double rad = theta * M_PI / 180.0;
    vertical += average * cos(rad);
    horizontal += average * sin(rad); // used to be thetaChange

    // vertical += ROBOTLENGTH * sin(thetaChange);
    // horizontal += ROBOTLENGTH * (1 - cos(thetaChange));

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
    if(abs(vertical) < 1)
    {
        vertical = 1;
    }
    returnVal = atan2(-horizontal, -vertical);
    returnVal *= (180 / 3.14159265358979323846);
    printf("ogreturnVal: %f\n", returnVal);

    ////returnVal *= (horizontal/abs(horizontal)) * (vertical/abs(vertical));
    //returnVal = (270 * (returnVal/abs(returnVal))) - (returnVal + theta);
    
    returnVal = returnVal - theta;
    //returnVal = (180 - (returnVal + theta));

    // returnVal += (90 * (horizontal/abs(horizontal)) * (vertical/abs(vertical))) - theta;
    printf("returnVal: %f\n", returnVal); 

    if(abs(returnVal) < 180)
    {
        returnVal = -1 * returnVal;
    }
    else
    {
        returnVal = (360 - abs(returnVal)) * (returnVal/abs(returnVal));
    }

    printf("final returnVal: %f\n", returnVal);
}

bool ReturnedAngleUpdated()
{
    float dL = TopLeft.position(vex::deg);
    float dR = TopRight.position(vex::deg);
    float thetaChange = ((dL - dR) * 2 * RADIUS * 3.14159265358979323846) / 360;
    thetaChange /= (2 * ROBOTLENGTH * 3.14159265358979323846);
    thetaChange *= 360;
    returnTheta += thetaChange;

    //printf("current: %f\n", returnTheta); 

    if(abs(returnTheta) >= abs(returnVal)) return true;

    if(returnVal < 0)
    {
        RunMotor( 20, TopLeft );
        RunMotor( 20, BottomLeft );
        RunMotor( -20, TopRight );
        RunMotor( -20, BottomRight );
    }
    else
    {
        RunMotor( -20, TopLeft );
        RunMotor( -20, BottomLeft );
        RunMotor( 20, TopRight );
        RunMotor( 20, BottomRight );
    }

    TopLeft.resetPosition();
    TopRight.resetPosition();
    BottomLeft.resetPosition();
    BottomRight.resetPosition();

    return false;
}

void CalculateStraightReturn()
{
    straightReturn = sqrt((horizontal * horizontal) + (vertical * vertical));
}

bool MoveToOrigin()
{
    if(abs(TopLeft.position(vex::deg)) >= straightReturn && abs(TopRight.position(vex::deg)) >= straightReturn) return true;
    
    RunMotor( 20, TopLeft );
    RunMotor( 20, BottomLeft );
    RunMotor( 20, TopRight );
    RunMotor( 20, BottomRight );
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

        if(Controller.ButtonA.pressing())
        {
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

    EstablishReturnVal();

    theta = 0;

    while(!ReturnedAngleUpdated()) this_thread::sleep_for(10);

    printf("brother what\n");

    TopLeft.resetPosition();
    TopRight.resetPosition();
    BottomLeft.resetPosition();
    BottomRight.resetPosition();

    CalculateStraightReturn();

    while(true)
    {
        if(MoveToOrigin()) break;

        this_thread::sleep_for(10);
    }


    RunMotor(0, TopLeft);
    RunMotor(0, BottomLeft);
    RunMotor(0, TopRight);
    RunMotor(0, BottomRight);
}

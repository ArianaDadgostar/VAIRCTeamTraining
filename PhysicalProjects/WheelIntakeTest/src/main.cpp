/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       arianadadgostar                                           */
/*    Created:      6/29/2026, 6:58:08 PM                                     */
/*    Description:  IQ2 project                                               */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

// A global instance of vex::brain used for printing to the IQ2 brain screen
vex::brain       Brain;
vex::controller Controller;
vex::motor topLeft = vex::motor(PORT5, false);
vex::motor topRight = vex::motor(PORT6, true);
vex::pneumatic sorter = vex::pneumatic(PORT1, true);

// define your global instances of motors and other devices here

enum class SortingPneumatics
{
    LEFT,
    RIGHT
};

enum ButtonPressed
{
    NotPressed,
    Pressed
};

class Button
{
    public:
        vex::controller::button button;
        ButtonPressed state;
        bool toggled;

        Button(vex::controller::button button)
        {
            toggled = false;
            state = NotPressed;
            this->button = button;
        }
};

#pragma region BasicProgramFunctions

bool IsBumped(Button &buttonInfo)
{
    switch(buttonInfo.state)
    {
        case NotPressed:
            if(buttonInfo.button.pressing())
            {
                buttonInfo.state = Pressed;
                return false;
            }
            break;
        case Pressed:
            if(!buttonInfo.button.pressing())
            {
                buttonInfo.state = NotPressed;
                buttonInfo.toggled = !buttonInfo.toggled;
                return true;
            }
            break;
    }
    return false;
}

void SetMotor(vex::motor motor, int power, int direction)
{
    motor.spin(vex::directionType::fwd, power * direction, vex::percentUnits::pct);
}

#pragma endregion

void RunSorter(Button &leftSorter, Button &rightSorter)
{
    IsBumped(leftSorter);
    IsBumped(rightSorter);
    vex::cylinderType cylinder;

    if(leftSorter.toggled)
    {
        sorter.extend(vex::cylinderType(SortingPneumatics::LEFT));
    }
    else{
        sorter.retract(vex::cylinderType(SortingPneumatics::LEFT));
    }

    if(rightSorter.toggled)
    {
        sorter.extend(vex::cylinderType(SortingPneumatics::RIGHT));
    }
    else{
        sorter.retract(vex::cylinderType(SortingPneumatics::RIGHT));
    }
}

void RunConveyor(Button &conveyor)
{
    IsBumped(conveyor);
    int power = (conveyor.toggled) ? 100 : 0;

    SetMotor(topLeft, power, 1);
    SetMotor(topRight, power, 1);
}

int main() {

    Button leftSorter = Button(Controller.ButtonFUp);
    Button rightSorter = Button(Controller.ButtonFDown);

    Button conveyor = Button(Controller.ButtonEUp);
    

    while(true) {
        sorter.pumpOn();
        RunSorter(leftSorter, rightSorter);
        RunConveyor(conveyor);
    }
}
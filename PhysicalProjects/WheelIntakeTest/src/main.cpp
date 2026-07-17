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

enum class MotorDirection
{
    FORWARD,
    OFF
};

enum ButtonPressed
{
    NotPressed,
    Pressed,
    Released,
    MannualEntry
};

class Button
{
    public:
        int power;
        vex::controller::button button;
        ButtonPressed state;
        bool toggled;
        timer mannual;

        Button(vex::controller::button button)
        {
            toggled = false;
            state = NotPressed;
            this->button = button;
        }
};

#pragma region BasicProgramFunctions

bool ButtonPressingLogic(Button &buttonInfo)
{
    switch (buttonInfo.state)
    {
    case NotPressed:
    {
        if (!buttonInfo.button.pressing()) break;
        
        buttonInfo.mannual.reset();
        buttonInfo.state = Pressed;

        break;
    }

    case Pressed:
    {
        if (!buttonInfo.button.pressing())
        {
            buttonInfo.state = Released;
            // if(buttonInfo.mannual.time() <= 1000) break;
            // buttonInfo.state = MannualEntry;
        }
        break;
    }

    case Released:
    {
        buttonInfo.toggled = !buttonInfo.toggled;
        buttonInfo.state = NotPressed;
        return true;
    }

    case MannualEntry:
    {
        if (buttonInfo.button.pressing())
        {
            buttonInfo.mannual.reset();
            buttonInfo.state = Pressed;
        }

        break;
    }
    }

    return false;
}

void SetMotor(vex::motor motor, int power, int direction)
{
    motor.spin(vex::directionType::fwd, power * direction, vex::percentUnits::pct);
}

#pragma endregion

void RunSorter()
{
    vex::cylinderType cylinder;
    if(!Controller.ButtonFUp.pressing() && !Controller.ButtonFDown.pressing())
    {
        sorter.extend(vex::cylinderType(SortingPneumatics::LEFT));
        sorter.extend(vex::cylinderType(SortingPneumatics::RIGHT));
        return;
    }
    else
    {
        cylinder = (Controller.ButtonFUp.pressing()) ? vex::cylinderType(SortingPneumatics::LEFT) : vex::cylinderType(SortingPneumatics::RIGHT);
    }
    sorter.retract(cylinder);
}

int main() {

    Button rightClaw = Button(Controller.ButtonRUp);

    Button lClawReset = Button(Controller.ButtonLDown);
    Button rClawReset = Button(Controller.ButtonRDown);

    MotorDirection direction = MotorDirection::FORWARD;

    
    sorter.pumpOn();

    while(true) {
        direction = (Controller.ButtonEUp.pressing()) ? MotorDirection::FORWARD : MotorDirection::OFF;
        switch(direction)
        {
            case MotorDirection::FORWARD:
                SetMotor(topLeft, 100, 1);
                SetMotor(topRight, 100, 1);
                break;
            case MotorDirection::OFF:
                SetMotor(topLeft, 0, 0);
                SetMotor(topRight, 0, 0);
                break;
        }
        RunSorter();
    }
}
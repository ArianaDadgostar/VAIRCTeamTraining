/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       arianadadgostar                                           */
/*    Created:      7/23/2026, 2:02:50 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
#include <bits/stdc++.h>

using namespace vex;
// using namespace std;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;
vex::controller Controller;
vex::motor LeftLift(vex::PORT11, false);
vex::motor RightLift(vex::PORT1, true);
vex::motor Claw(vex::PORT20, false);
vex::motor LeftChassis(vex::PORT2, false);
vex::motor RightChassis(vex::PORT3, true);
vex::motor ClawTurner(vex::PORT10, false);

// #pragma region BasicDefinitions

// enum PressState
// {
//     Pressed,
//     Released
// };

// enum Actions
// {
//     LiftUp,
//     LiftDown,
//     ClawOpen,
//     ClawClose,
// };

// map<controller::button, Actions> assignActions = 
// {
//     {Controller.ButtonR1, LiftUp},
//     {Controller.ButtonR2, LiftDown},
//     {Controller.ButtonL1, ClawOpen},
//     {Controller.ButtonL2, ClawClose},
// };

// class Button
// {
//     public:

//     controller::button button;
//     PressState state;
//     bool active;

//     Button(controller::button button)
//     {
//         this->button = button;
//         state = Released;
//         active = false;
//     }

//     Button ButtonPressed(Button& current)
//     {
//         switch(state)
//         {
//             case Pressed:
//             {
//                 if(button.pressing()) break;

//                 state = Released;
//                 current.active = true;
//                 current.button = button;
//                 return current;
//             }
//             case Released:
//             {
//                 if(!button.pressing()) break;

//                 state = Pressed;
//                 break;
//             }
//         }
//         return current;
//     }
// };

// #pragma endregion

// define your global instances of motors and other devices here

void SpinMotor(int power, vex::motor &Motor)
{
    Motor.spin(vex::directionType::fwd, power, vex::velocityUnits::pct);
}

// void Test(int power, Button& current)
// {
//     if(!current.active) return;
//     switch(assignActions[current.button])
//     {
//         case LiftUp:
//         {
//             SpinMotor(power, LeftLift);
//             SpinMotor(power, RightLift);
//             break;
//         }

//         case LiftDown:
//         {
//             SpinMotor(-power, LeftLift);
//             SpinMotor(-power, RightLift);
//             break;
//         }
        
//         case ClawOpen:
//         {
//             SpinMotor(power, Claw);
//             break;
//         }

//         case ClawClose:
//         {
//             SpinMotor(-power, Claw);
//             break;
//         }

//         default:
//         {
//             SpinMotor(0, Claw);
//             SpinMotor(0, LeftLift);
//             SpinMotor(0, RightLift);
//             break;
//         }
//     }
//     current.active = false; //Will not continue to move motor solve this later
// }

// void CheckButtons(Button& current, Button buttons[], int buttonNum)
// {
//     for(int i = 0; i < buttonNum; i++)
//     {
//         current = buttons[i].ButtonPressed(current);
//     }
// }

int main() {
    // Button current = Button(Controller.ButtonR1);
    // Button buttons[] = {
    //     Button(Controller.ButtonR1),
    //     Button(Controller.ButtonR2),
    //     Button(Controller.ButtonL1),
    //     Button(Controller.ButtonL2)
    // };

    while(1) {
        // CheckButtons(current, buttons, 4); THIS SHOULD BE HERE DONT DELETE
        LeftLift.setBrake(brakeType::hold);
        RightLift.setBrake(brakeType::hold);
        Claw.setBrake(brakeType::hold);
        ClawTurner.setBrake(brakeType::hold);

        if(Controller.ButtonR1.pressing())
        {
            SpinMotor(50, LeftLift);
            SpinMotor(50, RightLift);
        }
        else if(Controller.ButtonL1.pressing())
        {
            SpinMotor(50, Claw);
        }
        else if(Controller.ButtonR2.pressing())
        {
            SpinMotor(-50, LeftLift);
            SpinMotor(-50, RightLift);
        }
        else if(Controller.ButtonL2.pressing())
        {
            SpinMotor(-50, Claw);
        }
        else{
            SpinMotor(0, LeftLift);
            SpinMotor(0, RightLift);
            SpinMotor(0, Claw);
        }

        SpinMotor(Controller.Axis3.position(), LeftChassis);
        SpinMotor(Controller.Axis2.position(), RightChassis);
        if(Controller.ButtonA.pressing())
        {
            SpinMotor(50, ClawTurner);
        }
        else if(Controller.ButtonB.pressing())
        {
            SpinMotor(-50, ClawTurner);
        }
        else
        {
            SpinMotor(0, ClawTurner);
        }

        // Allow other tasks to run
        this_thread::sleep_for(10);
    }
}

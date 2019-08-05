
#include "SDL2/SDL.h"
#include <vector>
#include <structs.h>
#include <unistd.h>
#include <iostream>
#include <kbhit.h>
// int kbhit();

//BUTTON A 0 //BUTTON B 1 //BUTTON X 2 //BUTTON Y 3 //BUTTON HORIZONTAL LINES 6
//BUTTON DOUBLE SQUARE 4 //BUTTON XBOX 5 //BUTTON RIGHT SHOULDER 10 //BUTTON LEFT SHOULDER 9
//UP 11 //DOWN 12 //LEFT 13 //RIGHT 14

//Analog joystick dead zone
const int JOY_DEADZONE = 3200;

#define WAIT_COUNT 200

// struct  GamePad {
//     bool buttons[SDL_CONTROLLER_BUTTON_MAX];
//     int axis[SDL_CONTROLLER_AXIS_MAX];
// };

// bool Input::isControllerButtonPressed(Inputs::Controllers controllerID, SDL_GameControllerButton button);
#include <structs.h>
#include "SDL2/SDL.h"
#include <iostream>


#define WAIT_COUNT 200

//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;
bool XboxInit();

using namespace std;

int waitCount;

int main() {

    void* SMpm;
    PM* PMPtr; 
    waitCount = 0;
    std::cout << "Entered XboxMain" << std::endl;
    usleep(10000);
    SMpm = SMCreate(PM_KEY,sizeof(PM));
// Read from SM
    PMPtr = (PM*)SMpm;


    SDL_GameController* Controller;

    while(!PMPtr->Shutdown.Flags.Xbox) {
        usleep(500000); 
        std::cout << "Entered Xbox condition" << std::endl;
        if (PMPtr->Heartbeats.Flags.Xbox) {
            if (++waitCount > WAIT_COUNT) {
				std::cout << "Xbox shutting down PM" << std::cout;
				PMPtr->Shutdown.Status = 0xFF; // PM Failure
			}
        } else {
            PMPtr->Heartbeats.Flags.Xbox = 1;
            waitCount = 0;
        }
        std::cout << "Post heartbeat "  << std::endl;
        //Initialise controller
        SDL_Init(SDL_INIT_GAMECONTROLLER);
        int JoystickIndex = SDL_NumJoysticks();
        int ControllerIndex = 0;
        if (!SDL_IsGameController(JoystickIndex)) {
            std::cout << "No joystick" << std::endl;
        }
        Controller = SDL_GameControllerOpen(JoystickIndex);
        
        // Operations on open controllers
        bool IsAPressed = SDL_GameControllerGetButton(Controller, SDL_CONTROLLER_BUTTON_A);
        std::cout << "A is pressed? " << IsAPressed << std::endl;
       
    }
    if (Controller) 
        SDL_GameControllerClose(Controller);
   
    return 0;
}

bool XboxInit() {

}
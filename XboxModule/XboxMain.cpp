#include "Xbox.h"
 
using namespace std;

int waitCount;

int main() {

    void* SMpm;
    PM* PMPtr; 
    waitCount = 0;
    std::cout << "Entered XboxMain" << std::endl;
    usleep(10000);
    SMpm = SMCreate(PM_KEY,sizeof(PM));
    int numGamepads;
// Read from SM
    PMPtr = (PM*)SMpm;
	PMPtr->Shutdown.Flags.Xbox = 0;

    SDL_Joystick *joy;
    SDL_Event event;

    // Initialize the joystick subsystem
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    // Enable for events
    SDL_JoystickEventState(SDL_ENABLE);

    // Check for joystick
    if (SDL_NumJoysticks() > 0) {
        // Open joystick
        joy = SDL_JoystickOpen(0);

        if (joy) {
            printf("Opened Joystick 0\n");
            printf("Name: %s\n", SDL_JoystickNameForIndex(0));
            printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
            printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
            printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));
        } else {
            printf("Couldn't open Joystick 0\n");
        }
    }

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

        while(SDL_PollEvent(&event)) {  
                switch(event.type){  
                    case SDL_KEYDOWN:
                    /* handle keyboard stuff here */				
                    break;

                    case SDL_QUIT:
                    /* Set whatever flags are necessary to */
                    /* end the main game loop here */
                    break;

                    case SDL_JOYBUTTONDOWN:  /* Handle Joystick Button Presses */
                        switch(event.jbutton.button) {
                            case SDL_CONTROLLER_BUTTON_X:
                                std::cout << "Shutting down" << std::endl;
                                PMPtr->Shutdown.Status = 0xFF; // Shutdown xbox 
                        }
                        
                        if ( event.jbutton.button == 0 ) {
                        std::cout << "Button zero" << std::endl;
                            /* code goes here */
                        }
                        std::cout << "Button: " << (int)event.jbutton.button << std::endl;
                    break;

                    case SDL_JOYAXISMOTION:  /* Handle Joystick Motion */

                            switch(event.jaxis.axis) {
                                // case 0: //Left joystick L/R 
                                // std::cout << "Left/Right axis value is: " << event.jaxis.value << std::endl;

                                case 1: //Left joystick U/D = SPEED
                                std::cout << "Up/Down axis value is: " << event.jaxis.value << std::endl;
                                    if ( ( event.jaxis.value < -JOY_DEADZONE ) || (event.jaxis.value > JOY_DEADZONE ) ) 
                                    {
                                        PMPtr->RemoteSpeed = -event.jaxis.value/(pow(2,15));
                                        
                                    } else {
                                        PMPtr->RemoteSpeed = 0;
                                    }
                                    std::cout << "Ptr remoteSpeed = " << PMPtr->RemoteSpeed << std::endl;
                                break;
                                case 2: //Right joystick L/R = STEERING
                                std::cout << "Left/Right axis value is: " << event.jaxis.value << std::endl;
                                    if ( ( event.jaxis.value < -JOY_DEADZONE ) || (event.jaxis.value > JOY_DEADZONE ) ) 
                                    {
                                         PMPtr->RemoteSteering = 40.0*event.jaxis.value/(pow(2,15));
                                    } else {
                                        PMPtr->RemoteSteering = 0;
                                    }
                                    std::cout << "Ptr remoteSteering = " << PMPtr->RemoteSteering << std::endl;
                                break;
                                // case 3: //Right joystick U/D
                                // std::cout << "Up/Down axis value is: " << event.jaxis.value << std::endl;
                            } 
                    break;
                }
        }
        if (kbhit()) {
            break;
        }
    }
              // Close if opened
    if (SDL_JoystickGetAttached(joy)) {
        SDL_JoystickClose(joy);
    }
    std::cout << "Shutdown status high" << std::endl;
    int detRet = shmdt(SMpm);
    if (detRet != 0) {
        std::cout << "SMpm detach failed" << std::endl;
    }
    shmctl(PM_KEY, IPC_RMID, NULL); 
	return 0;
}
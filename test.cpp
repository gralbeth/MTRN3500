//  #include "SDL2/SDL.h"

// #include <unistd.h>
//  #include <iostream>
 
// int kbhit();

// int main() {
//     SDL_Joystick *joy;
//     SDL_Event event;

//     // Initialize the joystick subsystem
//     SDL_InitSubSystem(SDL_INIT_JOYSTICK);
//     // Enable for events
//     SDL_JoystickEventState(SDL_ENABLE);

//     // Check for joystick
//     if (SDL_NumJoysticks() > 0) {
//         // Open joystick
//         joy = SDL_JoystickOpen(0);

//         if (joy) {
//             printf("Opened Joystick 0\n");
//             printf("Name: %s\n", SDL_JoystickNameForIndex(0));
//             printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
//             printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
//             printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));
//         } else {
//             printf("Couldn't open Joystick 0\n");
//         }
//     }
//     while(1) {
//         while(SDL_PollEvent(&event))
//         {  
//                 switch(event.type)
//                 {  
//                     case SDL_KEYDOWN:
//                     /* handle keyboard stuff here */				
//                     break;

//                     case SDL_QUIT:
//                     /* Set whatever flags are necessary to */
//                     /* end the main game loop here */
//                     break;

//                     case SDL_JOYBUTTONDOWN:  /* Handle Joystick Button Presses */
//                         switch(event.jbutton.button) {
//                             case SDL_CONTROLLER_BUTTON_X
//                                 std::cout << "Shutting down" << std::endl;

//                         }
                
                        
//                         if ( event.jbutton.button == 0 ) 
//                         {
//                         std::cout << "Button zero" << std::endl;
//                             /* code goes here */
//                         }
//                         std::cout << "Button: " << (int)event.jbutton.button << std::endl;
//                     break;
//                 }
//         }
//         if (kbhit()) {
//             break;
//         }
//     }

//               // Close if opened
//         if (SDL_JoystickGetAttached(joy)) {
//             SDL_JoystickClose(joy);
//         }
//         return 0;
// }

// int kbhit() {
//     struct timeval tv = { 0L, 0L };
//     fd_set fds;
//     FD_ZERO(&fds);
//     FD_SET(0, &fds);
//     return select(1, &fds, NULL, NULL, &tv);
// }
 
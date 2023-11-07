#include <SDL2/SDL.h>
#ifndef EVENT_H
#define EVENT_H

/*
Event handler for SDL events
user should be able to get events whilst hiding SDL
polling events
options to combine multiple inputs into one event group, e.g. binding a,b,x,y all to "interact"

detectable events should include
- buttonpressed
- buttonheld
- mouseinput
- joystick button press
- joystick movement
*/

#define pollEvent(event) SDL_PollEvent(event)
#define KEY_CODE SDL_KeyCode

#define KEY_DOWN SDLK_DOWN
#define KEY_UP SDLK_UP
#define KEY_LEFT SDLK_LEFT
#define KEY_RIGHT SDLK_RIGHT
#define KEY_SPACE SDLK_SPACE


int isKeyDown(SDL_Event event);
int isKeyUp(SDL_Event event);
int isKeyHeld(SDL_Event event);

KEY_CODE getKeyPressed(SDL_Event event);
          
/*
- Set delay of time (milliseconds), store the delay in pointer var
- e.g: setDelay(&i, 100) sets of 100 miliseconds and stores it in i
*/
void setDelay (int* var, int time);
//ticks decrements value of pointer by 1, to represent a tick in the set delay.
void delayTick (int* var);
//returns 0 if delay stored at the adress is not over, 1 if it is
int delayPassed (int *var);

#endif
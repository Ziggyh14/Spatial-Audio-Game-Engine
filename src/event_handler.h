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
#endif
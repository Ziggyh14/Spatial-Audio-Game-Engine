#include "sdl_func.h"
#include "sample_func.h"
#include "state_manager.h"
#include "event_handler.h"
#include "header.h"

#define ui64 uint64_t
#define ui32 uint32_t

#define PLAYER_VEL 0.5
#define TRUE 1
#define FALSE 0

#define QUIT_CHECK if(SDL_QuitRequested()){break;}

float playerpos = 0;
int delay = 0;

ui32 update1(ui32 dT){

    SDL_Event e;
    while(pollEvent(&e)){
        if(isKeyDown(e)){
            switch(getKeyPressed(e)){
                case KEY_UP: playerpos = playerpos + (PLAYER_VEL * dT); break;
                case KEY_DOWN: playerpos = playerpos - (PLAYER_VEL * dT); break;
                default: break;
            }
        }
    }
    printf("%f\n",playerpos);
    return 0;
}

ui32 draw1(ui32 dT){
    
    if(playerpos == 50 && delayPassed(&delay)){
        setDelay(&delay,1000);
        play_Sample("bin/jazz.wav",0);
    }
    delayTick(&delay);
    return 0;
}

int main(int argc, char** argv){

    State s = {0};
    s.draw = &draw1;
    s.update = &update1;

    create_Table(); // must be called before init_window

    SDL_Window* w = init_Window("DEMO", 1280,720); //init sdl and create a window

    init_Sample_Playback(96000,MIX_DEFAULT_FORMAT,2048); //init sdl mixer, and reserve space for space queue

    play_Sample("bin/jazz.wav",0); //play a sample from file, play loop+1 times

    ui32 lastTick = 0; // stores the last time for dT calc 
    ui32 dT = 0; // variable for delta time used for physics calculations

    while (TRUE){ // game loop to run indefinitely unless quit

        QUIT_CHECK; // alias for detecting if a quit event is queued and breaks if there is

        dT = SDL_GetTicks() - lastTick; // dT is time passed between now and completion of last frame
        lastTick = SDL_GetTicks(); // set last tick for next loop

        //STATE FUNCTIONS
        s.update(dT); // update anything before "drawing" 
        s.draw(dT); // "draw" anything that needs drawing
    }
    close_Sample_Playback(); // close sample playback and delete the hash table
    close_Window(w); // close window and exit SDL
}


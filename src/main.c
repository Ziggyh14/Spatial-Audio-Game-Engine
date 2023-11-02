#include "sdl_func.h"
#include "sample_func.h"
#include "state_manager.h"
#include "event_handler.h"


#define ui64 uint64_t
#define ui32 uint32_t

#define PLAYER_VEL 0.5
#define TRUE 1
#define FALSE 0

#define QUIT_CHECK if(SDL_QuitRequested()){break;}

float playerpos = 0;
int check = 0;


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

ui32 draw1(){
    if(playerpos == 50 && check == 0){
        play_Sample("bin/jazz.wav",0);
        check = 1;
    }
    return 0;
}

int main(int argc, char** argv){

    State s = {0};
    s.draw = &draw1;
    s.update = &update1;

    create_Table(); // must be called before init_window

    SDL_Window* w = init_Window("DEMO", 1280,720); //init sdl and create a window

    init_Sample_Playback(); //init sdl mixer, and reserve space for space queue

    //play_Sample("bin/jazz.wav",0); //play a sample from file, play loop+1 times

    ui32 lastTick = 0; // stores the last time for dT calc 
    ui32 dT = 0; // variable for delta time used for physics calculations

    while (TRUE){ // game loop to run indefinitely unless quit

        QUIT_CHECK; // alias for detecting if a quit event is queued and breaks if there is

        dT = SDL_GetTicks() - lastTick; // dT is time passed between now and completion of last frame
        lastTick = SDL_GetTicks(); // set last tick for next loop

        //STATE FUNCTIONS
        s.update(dT); // update anything before "drawing" 
       // s.draw(); // "draw" anything that needs drawing
    }
    close_Sample_Playback(); // close sample playback and delete the hash table
    close_Window(w); // close window and exit SDL
    
}

/*
    create_Table();
    if (ht == NULL){
        return 1;
    }

    
    SDL_Window* window = init_Window("DEMO", 1280,720);
    
    init_Sample_Playback();
    printf("%d\n",queue_Count);
    SampleQueue* q = init_Queue();

    printf_Q(q);

    enqueue_Sample("bin/jazz.wav",-1,q);
    enqueue_Sample("bin/door.wav",-1,q);
    enqueue_Sample("bin/jazz.wav",-1,q);

   // play_Sample("bin/bassloop.wav",0);

    print_ht();

    SDL_Event e;
    int quit = 0;
    while(!quit){
        handle_Queue(q);
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                quit = 1;
            }
            if(e.type == SDL_KEYDOWN){
                
            }
        }
    }

    free(q);
    close_Sample_Playback();

    close_Window(window);
*/

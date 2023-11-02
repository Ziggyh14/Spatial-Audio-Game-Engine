#include "sdl_func.h"
#include "sample_func.h"
#include "state_manager.h"
#include "event_handler.h"

#define TRUE 1
#define FALSE 0
#define ui64 uint64_t
#define ui32 uint32_t

#define QUIT_CHECK if(SDL_QuitRequested()){break;}

int playerpos = 0;
int check = 0;


ui32 update1(ui32 dT){

    SDL_Event e;
    while(pollEvent(&e)){
        if(isKeyDown(e)){
            switch(getKeyPressed(e)){
                case KEY_UP: playerpos++; break;
                case KEY_DOWN: playerpos--; break;
                default: break;
            }
            printf("%d\n",playerpos);

        }
    }

    return 0;
}

unsigned int draw1(){
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

    SDL_Window* w = init_Window("DEMO", 1280,720);

    init_Sample_Playback();

    play_Sample("bin/jazz.wav",0);

    int running = TRUE;
    ui32 lastTick = 0;
    ui32 dT = 0;

    while (running){

        QUIT_CHECK;

        dT = SDL_GetTicks() - lastTick;
        lastTick = SDL_GetTicks();

        //STATE FUNCTIONS
        s.update(dT);
        s.init();
    }
    close_Sample_Playback();
    close_Window(w);
    
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

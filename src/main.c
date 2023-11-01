#include "sdl_func.h"
#include "sample_func.h"



int main(int argc, char** argv){

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
}


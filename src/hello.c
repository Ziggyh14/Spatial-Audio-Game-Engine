#include "hello.h"


SDL_Window* init_Window (const char* title, int width, int height){

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)){
        printf("error initializing SDL: %s\n", SDL_GetError());
        return NULL;
        }

    return SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED_DISPLAY(0),
            SDL_WINDOWPOS_CENTERED_DISPLAY(0),
            width,
            height,
            SDL_WINDOW_ALLOW_HIGHDPI);
}

void close_Window(SDL_Window* window){

    SDL_DestroyWindow(window);
    SDL_Quit();
}


int main(int argc, char** argv){

    Mix_Chunk* c;
    SDL_Window* window = init_Window("DEMO", 1280,720);
    
    //Mixer test
    Mix_OpenAudio(
        MIX_DEFAULT_FREQUENCY,
        MIX_DEFAULT_FORMAT,
        2,4096);
    c = Mix_LoadWAV("bin/bassloop.wav");
    Mix_AllocateChannels(1);
    //printf("%d\n", Mix_Volume(-1,MIX_MAX_VOLUME));

    SDL_Event e;
    int quit = 0;
    while(!quit){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                quit = 1;
            }
            if(e.type == SDL_KEYDOWN){
                Mix_PlayChannel(0 ,c,0);

            }
        }
    }
    Mix_FreeChunk(c);
    Mix_CloseAudio();
    close_Window(window);
    return 0;
}
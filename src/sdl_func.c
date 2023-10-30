#include "sdl_func.h"


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

Mix_Chunk* play_Sample(const char* file){
    Mix_Chunk* temp = Mix_LoadWAV(file);
    Mix_PlayChannel(-1 ,temp,0);
    return temp;
}


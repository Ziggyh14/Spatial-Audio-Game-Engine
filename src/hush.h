#ifndef __HUSH_ENGINE__
#define __HUSH_ENGINE__

#include "sample_func.h"
#include "sdl_func.h"
#include "state_manager.h"
#include "collision_util.h"

int hsh_init(uint16_t format, uint32_t rate){

    SDL_Init(SDL_INIT_AUDIO);
    hsh_initSamplePlayback(format,rate);
}

int hsh_close(){
    SDL_Quit();
    hsh_closeSamplePlayback();
}

#endif
#include "sample_func.h"


void init_Sample_Playback(){ //todo make init variables parameters

    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,2,4096);
}

/*
TODO:
    -add flags for saving/forgetting about the sample (default save)
*/
int play_Sample(const char* file){ 
    Entry* e = hash_lookup(file);
    if(e==NULL)
        return 1;
    
    Mix_PlayChannel(-1 ,e->chunk,0);   
    return 0;
}


#include "sample_func.h"

/*
TODO:
    -remove ht from functio 
    -add flags for saving/forgetting about the sample (default save)
*/

int play_Sample(const char* file,int flag,Table* ht){ 
    Entry* e = hash_lookup(file,ht);
    Mix_PlayChannel(-1 ,e->chunk,0);   
    return 0;
}

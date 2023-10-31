#include "sample_func.h"


void init_Sample_Playback(){ //todo make init variables parameters

    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,2,4096);
    Mix_ReserveChannels(1);
}

/*
TODO:
    -add flags for saving/forgetting about the sample (default save)
*/
int play_Sample_Timed(const char* file,int loops, int mtime){ 
    Entry* e = hash_lookup(file);
    if(e==NULL)
        return 1;
    
    Mix_PlayChannelTimed(-1 ,e->chunk,loops,mtime);   
    return 0;
}


int queue_sample(const char* file, int mtime, SampleQueue* sq){

    SampleInfo* si = (SampleInfo*)malloc(sizeof(SampleInfo));
    Entry *e = hash_lookup(file);
    if(e==NULL){
        free(si);
        return -1;
    }
    si->mtime = mtime;
    si->chunk = e->chunk;
    si->loop  = 0;
    if(sq->head == NULL){
        sq->head = si;
        sq->tail = si;
        sq->length = 1;
        return sq->length;
    }
    
    sq->tail->next = si;
    sq->tail = si;
    sq->length += 1;
    return sq->length;
   
}

int dequeue_sample(SampleQueue* sq){

    if(sq->length==0)
        return;

    SampleInfo* s = sq->head;
    sq->head = sq->head->next;
    sq->length -= 1;

    free(s);
    return;
    
}

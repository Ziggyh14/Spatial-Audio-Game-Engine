#include "sample_func.h"


void init_Sample_Playback(){ //todo make init variables parameters

    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,2,4096);
    Mix_AllocateChannels(DEFAULT_CHANNEL_NO+1); // Allocate channels + queue buffer
    Mix_ReserveChannels(1); // Reserve a buffer channel for a possible queue to go
}

int play_Sample_Timed(const char* file,int loops, int mtime){ 
    Entry* e = hash_lookup(file);
    if(e==NULL)
        return 1;
    
    Mix_PlayChannelTimed(-1 ,e->chunk,loops,mtime);   
    return 0;
}

SampleQueue* init_Queue(){

    /*
    Alloc space in memory
    Assign it reserved channel
    Reserve new buffer channel
    return new queue
    */
    SampleQueue* q = (SampleQueue*) malloc(sizeof(SampleQueue));
    q->channel = queue_Count++;
    
    Mix_AllocateChannels(DEFAULT_CHANNEL_NO+1+queue_Count);
    Mix_ReserveChannels(queue_Count+1);
     
    return q;

}

int queue_Sample(const char* file, int mtime, SampleQueue* sq){

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

void dequeue_Sample(SampleQueue* sq){

    if(sq->length==0)
        return;

    SampleInfo* s = sq->head;
    sq->head = sq->head->next;
    sq->length -= 1;
    free(s);
    return;
    
}

void handle_Queue(SampleQueue* sq){

    if(sq->length == 0)
        return;

    if(Mix_Playing(sq->channel))
        return;
    
    SampleInfo* h = sq->head;
    Mix_PlayChannelTimed(sq->channel, h->chunk,0,h->mtime);
    dequeue_Sample(sq);

    return;
}

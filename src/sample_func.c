#include "sample_func.h"

int queue_Count; // Amount of queues initialised

void init_Sample_Playback(int freq, Uint16 format, int chunksize){ //todo make init variables parameters
    
    Mix_OpenAudio(freq,format,2,chunksize);
    Mix_AllocateChannels(DEFAULT_CHANNEL_NO+1); // Allocate channels + queue buffer
    //printf("Allocated channels %d\n", Mix_AllocateChannels(DEFAULT_CHANNEL_NO+1));
    queue_Count = 0;
    Mix_ReserveChannels(1); // Reserve a buffer channel for a possible queue to go
}

void close_Sample_Playback(){

    Mix_CloseAudio();
    delete_Table(); //delete the table
}

extern int play_Sample_Timed_inChannel(const char* file, int loops, int mtime,int channel){
    
    Entry* e = hash_lookup(file);
    if(e==NULL)
        return 1;
    if(channel > DEFAULT_CHANNEL_NO){
        printf("Error: %d channels not allocated\n",channel);
        return 1;
    }

    Mix_PlayChannelTimed(channel ,e->chunk,loops,mtime);   
    return 0;
}

int sample_Volume(const char* file, int volume){
    Entry* e = hash_lookup(file);
    return Mix_VolumeChunk(e->chunk,volume);
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
    q->length = 0;
    q->head = NULL;
    q->tail = NULL;
    Mix_AllocateChannels(DEFAULT_CHANNEL_NO+1+queue_Count);
    Mix_ReserveChannels(queue_Count+1);
     
    return q;

}

void free_Queue(SampleQueue* sq){

    if(sq->head == NULL){
        free(sq);
        return;
    }
    
    SampleInfo* si = sq->head;

    while(si!=NULL)
        dequeue_Sample(sq);
    
    free(sq);
    return;
}

int enqueue_Sample(const char* file, int mtime, SampleQueue* sq){

    SampleInfo* si = (SampleInfo*)malloc(sizeof(SampleInfo));
    Entry *e = hash_lookup(file);
    if(e==NULL){
        free(si);
        return -1;
    }

    si->mtime = mtime;
    si->file = file;
    si->next = NULL;

    if(sq->head == NULL){
        sq->head = si;
        sq->tail = si;
        sq->length = 1;
        printf_Q(sq);
        return sq->length;
    }
    
    sq->tail->next = si;
    sq->tail = si;
    sq->length += 1;
    printf_Q(sq);
    return sq->length;
   
}

void dequeue_Sample(SampleQueue* sq){

    if(sq->length==0)
        return;

    SampleInfo* s = sq->head;
    sq->head = sq->head->next;
    sq->length -= 1;
    printf_Q(sq);
    free(s);
    return;
}

void handle_Queue(SampleQueue* sq){

    if(sq->length == 0)
        return;

    if(Mix_Playing(sq->channel))
        return;
    
    printf("Play sample on channel %d\n",sq->channel);
    Entry* e = hash_lookup(sq->head->file);
    Mix_PlayChannelTimed(sq->channel, e->chunk,0,sq->head->mtime);
    dequeue_Sample(sq);

    return;
}

void printf_Q(SampleQueue* sq){

    printf("QUEUE LENGTH: %d, CHANNEL: %d\n",sq->length,sq->channel);
    
    if(sq->head != NULL){
        SampleInfo* s = sq->head;
        printf("%s",s->file);
        while(s->next != NULL){
            s=s->next;
            printf(", %s",s->file);
        }
        printf("\n");
    }
    return;
}

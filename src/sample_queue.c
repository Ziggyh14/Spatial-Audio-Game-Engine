#include "sample_queue.h"

hsh_SampleQueue* hsh_initQueue(){

    hsh_SampleQueue* sq = malloc(sizeof(hsh_SampleQueue));
    sq->head = NULL;
    sq->tail = NULL;
    sq->length = 0;
    sq->delayvar = 0;
    sq->current = -1;

    return sq;
}

void hsh_freeQueue(hsh_SampleQueue* sq){

    hsh_QueueEntry* qe = sq->head;
    hsh_QueueEntry* temp;
    while(qe!=NULL){
       temp = qe->next;
       free(qe);
       qe = temp;
    }
    free(sq);
    return;

}

extern uint8_t hsh_enqueueSample(hsh_SampleQueue* sq, Sound_Sample* sample,hsh_aSource* hsh_src, int16_t loops, int32_t mtime){

    hsh_QueueEntry* qe = malloc(sizeof(hsh_QueueEntry));
    qe->hsh_src = hsh_src;
    qe->mtime = mtime;
    qe->loops = loops;
    qe->sample = sample;
    qe->next = NULL;

    if(sq->head != NULL){
        sq->tail->next = qe;
        sq->tail = qe;
    }
    else{
        sq->head = qe;
        sq->tail = qe;
    }
    return ++(sq->length);
}

extern uint8_t hsh_enqueueSampleFromFile(const char* file,
                                         hsh_SampleQueue* sq, 
                                         hsh_aSource* hsh_src,
                                         int16_t loops,
                                         int32_t mtime
                                         )
{

    Entry* e = hash_lookup(file);
    return hsh_enqueueSample(sq, e->sample,hsh_src,loops,mtime);
}

int8_t hsh_enqueueDelay(int32_t time, hsh_SampleQueue* sq){
    hsh_QueueEntry* qe = malloc(sizeof(hsh_QueueEntry));
    qe->hsh_src = NULL;
    qe->sample = NULL;
    qe->mtime = time;
    qe->next = NULL;

    if(sq->head != NULL){
        sq->tail->next = qe;
        sq->tail = qe;
    }
    else{
        sq->head = qe;
        sq->tail = qe;
    }
    return ++(sq->length);
}

uint8_t hsh_dequeueSample(hsh_SampleQueue* sq){

    hsh_QueueEntry* qe = sq->head;
    sq->head = qe->next;
    free(qe);
    return --(sq->length);
}

void hsh_handleQueue(hsh_SampleQueue* sq){

    if(!(sq->head)){
        return;
    }

    if(sq->head->hsh_src == NULL){

        sq->delayvar += sq->head->mtime;
        hsh_dequeueSample(sq);
        hsh_handleQueue(sq);
    }

    ALint state;
    alAssert(alGetSourcei(sq->head->hsh_src->alSource,AL_SOURCE_STATE,&state));
    
    if(state == AL_PLAYING && sq->current == sq->head->hsh_src->alSource){
       
        hsh_feedSource(sq->head->hsh_src);
        return;
    }

    if((state == AL_STOPPED || state == AL_INITIAL) && sq->current == sq->head->hsh_src->alSource){
        
        hsh_dequeueSample(sq);
        sq->current = -1;
        hsh_handleQueue(sq);
        return;
    }

    if(state == AL_PLAYING && sq->current != sq->head->hsh_src->alSource){
        
        sq->current = sq->head->hsh_src->alSource;
        hsh_playSound(sq->head->sample,sq->head->hsh_src,sq->head->loops,sq->head->mtime);
        return;
    }

    if((state == AL_STOPPED || state == AL_INITIAL)&& sq->current != sq->head->hsh_src->alSource){
        
        sq->current = sq->head->hsh_src->alSource;
        hsh_playSound(sq->head->sample,sq->head->hsh_src,sq->head->loops,sq->head->mtime);
        return;
    }
}

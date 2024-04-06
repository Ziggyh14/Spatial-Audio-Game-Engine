#pragma once

#ifndef SAMPLE_Q_H
#define SAMPLE_Q_H

#include "sample_func.h"


typedef struct hsh_QueueEntry{
    hsh_aSource* hsh_src;
    Sound_Sample* sample;
    int16_t loops;
    int32_t mtime;
    struct hsh_QueueEntry* next;
} hsh_QueueEntry;

typedef struct hsh_SampleQueue{
    hsh_QueueEntry* head;
    hsh_QueueEntry* tail;
    ALuint current;
    int8_t length;
    int32_t delayvar;
} hsh_SampleQueue;

/*
- Initialse a SampleQueue and return a pointer to it
- samples in the queue are played on thier own channel, meaning other samples can be played without disturbing the queue
*/
extern hsh_SampleQueue* hsh_initQueue();

extern void hsh_freeQueue(hsh_SampleQueue* sq);
/*
push sample to queue, plays after all samples pushed before it.
- file: file location of the sample
- mtime: max time a sample will play until it is cut off. 
- queue to push to (array of string constants)
sample may end before mtime has elapsed if it (and its loops) are shorter than mtime
  -1 will let sample play out entirely.
- returns queue position on sucess, -1 on failure.
*/
extern uint8_t hsh_enqueueSample(hsh_SampleQueue* sq, Sound_Sample* sample,hsh_aSource* hsh_src, int16_t loops, int32_t mtime);

extern uint8_t hsh_enqueueSampleFromFile(const char* file,
                                         hsh_SampleQueue* sq, 
                                         hsh_aSource* hsh_src,
                                         int16_t loops,
                                         int32_t mtime
                                         );

extern int8_t hsh_enqueueDelay(int32_t time, hsh_SampleQueue* sq);

extern uint8_t hsh_dequeueSample(hsh_SampleQueue* sq);



/*
Handles playing from a given SampleQueue
- Will play the next sample in the queue 
if the queue is not empty nothing the queue is playing is playing
- Then dequeue it
*/
void hsh_handleQueue(hsh_SampleQueue* sq);

void printf_Q(hsh_SampleQueue* sq);







#endif
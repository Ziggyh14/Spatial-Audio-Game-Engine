#include <SDL2/SDL.h>
#include "sample_hash.h"
#include "event_handler.h"


#ifndef SAMPLE_FUNC_H
#define SAMPLE_FUNC_H

#define alAssert(function) {function; if(alGetError() != ALC_NO_ERROR) printf("ALC ERROR\n"); }

#define DEFAULT_CHANNEL_NO 16
#define NUM_BUFFERS 4

//Source defaults
#define SOURCE_PITCH_DEFAULT 1
#define SOURCE_GAIN_DEFAULT  1.0f
#define SOURCE_POS_DEFAULT   {0,0,0}
#define SOURCE_VEL_DEFAULT   {0,0,0}
#define SOURCE_LOOP_DEFAULT  0

typedef struct hush_AudioSource{
	ALuint source;
	ALuint buffers[4];
  Entry* entry;
} hush_AudioSource;

typedef struct hush_Vector3{
	float x,y,z;
} hush_Vector3;

extern hush_AudioSource* hush_init_Source(ALfloat pitch,ALfloat gain,hush_Vector3 position,hush_Vector3 velocity);

extern void init_Sample_Playback( Uint16 format,Uint32 rate);

extern void close_Sample_Playback();

/*
- Plays a sample from a given file,
- If it has not already been played it will allocate memory
- If it has already been used it will NOT reallocate new memory
- will play sample (loops+1) times
- will cut off sample after mtime ms, if sample length is longer
- RETURNS 0 on success, 1 on failure
*/
/*
extern int play_Sample_Timed_inChannel(const char* file, int loops, int mtime,int channel);

#define play_Sample_Timed(file,loops,mtime) play_Sample_Timed_inChannel(file,loops,mtime,-1);

#define play_Sample(file,loops) play_Sample_Timed_inChannel(file,loops,-1,-1);

*/

extern int hush_playSoundAtSource(const char* file,hush_AudioSource* source,int16_t loops);

extern void feed_source(hush_AudioSource* hsh_src);


/*
- Sets volume of sample given by filename.
- Safe to use even if file has not been played before.
- passing voume as -1 will return the existing volume of the sample and not change it.
*/
//int sample_Volume(const char* file, int volume);


typedef struct SampleInfo{
    const char* file;
    int mtime;
    int delay_flag;
    struct SampleInfo* next;
} SampleInfo;

typedef struct SampleQueue{
    SampleInfo* head;
    SampleInfo* tail;
    int length;
    int channel;
    int delayvar;
} SampleQueue;

int queue_Count; 

/*
- Initialse a SampleQueue and return a pointer to it
- samples in the queue are played on thier own channel, meaning other samples can be played without disturbing the queue
*/
SampleQueue* init_Queue();
void free_Queue(SampleQueue* sq);
/*
push sample to queue, plays after all samples pushed before it.
- file: file location of the sample
- mtime: max time a sample will play until it is cut off. 
- queue to push to (array of string constants)
sample may end before mtime has elapsed if it (and its loops) are shorter than mtime
  -1 will let sample play out entirely.
- returns queue position on sucess, -1 on failure.
*/
int enqueue_Sample(const char* file, int mtime, SampleQueue* sq);

int enqueue_Delay(int time, SampleQueue* sq);

void dequeue_Sample(SampleQueue* sq);

/*
Handles playing from a given SampleQueue
- Will play the next sample in the queue 
if the queue is not empty nothing the queue is playing is playing
- Then dequeue it
*/
void handle_Queue(SampleQueue* sq);

void printf_Q(SampleQueue* sq);
#endif

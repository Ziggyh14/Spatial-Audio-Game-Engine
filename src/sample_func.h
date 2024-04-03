#include <SDL2/SDL.h>
#include "sample_hash.h"
#include "event_handler.h"


#ifndef SAMPLE_FUNC_H
#define SAMPLE_FUNC_H

#define alAssert(function) {function; if(alGetError() != ALC_NO_ERROR) printf("ALC ERROR\n"); }

#define DEFAULT_CHANNEL_NO 16
#define NUM_BUFFERS 8+1

//Source defaults
#define SOURCE_PITCH_DEFAULT 1
#define SOURCE_GAIN_DEFAULT  1.0f
#define SOURCE_POS_DEFAULT   {0,0,0}
#define SOURCE_VEL_DEFAULT   {0,0,0}
#define SOURCE_LOOP_DEFAULT  0
#define SOURCE_REFERENCE_DEFAULT 0.1f
#define SOURCE_ROLLOFF_DEFAULT 0.5f
#define HUSH_STATE_PLAYING 1
#define HUSH_STATE_STOPPED 0

typedef struct hsh_aSource{
	ALuint alSource;
	ALuint* buffers;
  Sound_Sample* sample;
  int16_t loops;
  int32_t mtime;
  int8_t buffer_n;
} hsh_aSource;

typedef struct hsh_vec3{
	float x,y,z;
} hsh_vec3;

extern hsh_aSource* hush_init_Source(ALfloat pitch,ALfloat gain,hsh_vec3 position,hsh_vec3 velocity);

extern void hsh_freeSource(hsh_aSource* hsh_src);

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

extern ALuint hsh_bufferMath(hsh_aSource* hsh_src ,ALuint* buffers , Sound_Sample* sample, ALuint n);

extern int hsh_playSound(Sound_Sample* sample,hsh_aSource* src,int16_t loops, int32_t mtime);

extern int hsh_playSoundFromFile(const char* file,hsh_aSource* src,int16_t loops, int32_t mtime);

extern int hsh_pauseSource(hsh_aSource* src);

extern int hsh_unpauseSource(hsh_aSource* src);

extern int8_t feed_source(hsh_aSource* hsh_src);

extern int hsh_moveSource(hsh_aSource* hsh_src, hsh_vec3 pos);

extern void init_Sample_Playback( Uint16 format,Uint32 rate);

extern void close_Sample_Playback();

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

int queue_Count; 

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
extern uint8_t hsh_enqueueSample(hsh_SampleQueue* sq, Sound_Sample* sample, int32_t mtime, int16_t loops ,hsh_aSource* hsh_src);

extern uint8_t hsh_enqueueSampleFromFile(const char* file,
                                         hsh_SampleQueue* sq, 
                                         int32_t mtime, 
                                         int16_t loops,
                                         hsh_aSource* hsh_src);

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

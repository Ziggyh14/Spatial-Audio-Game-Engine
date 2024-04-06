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
} hsh_aSource;

typedef struct hsh_vec3{
	float x,y,z;
} hsh_vec3;

extern hsh_aSource* hush_initSource(ALfloat pitch,ALfloat gain,hsh_vec3 position,hsh_vec3 velocity);

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

extern int8_t hsh_feedSource(hsh_aSource* hsh_src);

extern int hsh_moveSource(hsh_aSource* hsh_src, hsh_vec3 pos);

extern void hsh_initSamplePlayback( Uint16 format,Uint32 rate);

extern void hsh_closeSamplePlayback();


#endif

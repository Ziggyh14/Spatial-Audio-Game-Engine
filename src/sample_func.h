#include <SDL2/SDL.h>
#include "sample_hash.h"
#include "event_handler.h"


#ifndef SAMPLE_FUNC_H
#define SAMPLE_FUNC_H

/**
 * used to assert an openAL function, and notify if there was on error
*/
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

/**
* Creates new hush audio source with given initial values for: pitch, gain position and velocity
* Allocates memory for struct data and (NUM_BUFFERS) audio buffers, which can be free'd with hsh_freeSource(hsh_aSource* hsh_src)
*
*   \param pitch initial pitch of the sound played from this source
*   \param gain  initial gain of the sound played from this source
*   \param position initial spatial position of sound played from this source
*   \param velocity initial velocity of sound played from this source
*   
*   \return pointer to newly created hsh_aSource object. NULL on error
*   
*   \sa hsh_freeSource
*/
extern hsh_aSource* hsh_initSource(ALfloat pitch,ALfloat gain,hsh_vec3 position,hsh_vec3 velocity);

/**
 * stops playback of given audio source and frees associated buffers and memory created by hsh_initSource(...)
 * sources that have already stopped playback can still be freed
 * 
 *  \param hsh_src audio source to stop and free
 * 
 *  \sa hsh_initSource
 * 
*/
extern void hsh_freeSource(hsh_aSource* hsh_src);

/**
 * auxiliary function for hsh_playSound(..) and hsh_feedSource(..) and should not be used on its own and can be ignored. 
 * loads n BUFFER_SIZE memory chunks from given sample into given buffers 
 *          
 * n buffers may not be loaded if the sample reaches EOF and shouldnt loop, or reaches maxinum play time. 
 * in which case memory will only be stored to x buffers, 
 * x is returned and should be used to queue only as many buffers that memory was stored in. 
 *  
 *  \param hsh_src source that sample is being played on, used to access mtime and loop values
 *  \param buffers pointer to buffer locations in memory, this will usually be hsh_src->buffers but doesnt have to be
 *  \param sample sample to retreive memory from
 *  \param n number of buffers to try to write to
 * 
 *  \return number of buffers sucessfully written to, likely used to queue to the corresponding openAL source
 * 
 *  \sa hsh_feedSource 
 *  \sa hsh_playSound
*/
extern ALuint hsh_bufferMath(hsh_aSource* hsh_src ,ALuint* buffers , Sound_Sample* sample, ALuint n);

/**
 * plays given SDL_Sound sample at source, for given loops and only for as long as mtime 
 * is called by hsh_playSoundFromFile, using the sample created or retrieved from hash table
 * this function will only queue up (NUM_BUFFERS * BUFFER_SIZE) audio data to be played at source,
 * the rest of the audio will only be played if new buffers are queued using hsh_feedSource(..) 
 * which will use given loops and mtime as expected
 * if a sample is already playing at source it will be stopped and overwritten by new sample
 * in order to queue samples see sample_queues
 * 
 *  \param sample SDL_Sound sample to play from
 *  \param src audio source to play sample at
 *  \param loops amount of times sample should loop,
 *               loops = 0 will play the sample once, 
 *               loops = 1 will play the sample twice
 *               loops = -1 will play the sample indefinitely or until mtime passes
 *  \param mtime maximum time (in milliseconds) the sample should play for. 
 *               sample will play until the sample finishes (loops included) or until mtime elsapses, whichever is soonest.
 *               mtime = 1000 will play the sample for a maximum of 1000 milliseconds
 *               mtime = -1 will play the sample for its entire duration (loops times) 
 *  \return uhhhh
 * 
 *  \sa hsh_feedSource 
 *  \sa hsh_playSoundFromFile
 */
extern int hsh_playSound(Sound_Sample* sample,hsh_aSource* src,int16_t loops, int32_t mtime);

/**
 * calls hsh_playSound with src,loops and mtime, using the SDL_Sound sample created or retireved from hash table using given file
 * returns non zero on success, 0 on failure to find a valid sample at given file path or corresponding entry in sample hash table
 * 
 *  \param file file to find new sample at or to query existing samples in sample hash table 
 *  \param src audio source to play sample at
 *  \param loops amount of times sample should loop,
 *               loops = 0 will play the sample once, 
 *               loops = 1 will play the sample twice
 *               loops = -1 will play the sample indefinitely or until mtime passes
 *  \param mtime maximum time (in milliseconds) the sample should play for. 
 *               sample will play until the sample finishes (loops included) or until mtime elsapses, whichever is soonest.
 *               mtime = 1000 will play the sample for a maximum of 1000 milliseconds
 *               mtime = -1 will play the sample for its entire duration (loops times) 
 *  \return uhhhh
 * 
 *  \sa hsh_playSound
*/
extern int hsh_playSoundFromFile(const char* file,hsh_aSource* src,int16_t loops, int32_t mtime);

/**
 * pauses playback at source, can be resumed with hsh_unpauseSource
 * 
 *  \param src audio source to pause playback at
 * 
 *  \sa hsh_unpauseSource
*/
extern int hsh_pauseSource(hsh_aSource* src);

/**
 * resumes playback at source if paused by hsh_pauseSource, otherwise has no effect
 * 
 * \param src audio source to resume playback at
 * 
 * \sa hsh_pauseSource
*/
extern int hsh_unpauseSource(hsh_aSource* src);

/**
 * used to feed more sample data to be played at source.
 * Must only be called after using hsh_playSound to begin sample playback otherwise will result in error. 
 * Must be called to keep the sample playing after initial buffers are loaded
 * If safe to call every frame in a while loop, will only act with there are spent buffers to unqueued and more sample data to be queued in
 * 
 *  \param hsh_src source to feed more sample data into
 *  \return number of new buffers just queued to be played at source
 *  
 *  \sa hsh_playSound
*/
extern int8_t hsh_feedSource(hsh_aSource* hsh_src);

extern int hsh_moveSource(hsh_aSource* hsh_src, hsh_vec3 pos);

extern void hsh_initSamplePlayback( Uint16 format,Uint32 rate);

extern void hsh_closeSamplePlayback();


#endif

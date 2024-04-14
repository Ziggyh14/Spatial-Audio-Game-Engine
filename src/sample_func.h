#include <SDL2/SDL.h>

#include "al.h"
#include "alc.h"
#include "alext.h"
#include "SDL_sound.h"
#ifndef SAMPLE_FUNC_H
#define SAMPLE_FUNC_H

/**
 * used to assert an openAL function, and notify if there was on error
*/
#define alAssert(function,r) {function; if(alGetError() != ALC_NO_ERROR) {hsh_getALError(); return r;}}

static void hsh_getALError(){

    const char* s;
    switch(alGetError()){
        case AL_INVALID_NAME: 
            s = "a bad name (ID) was passed to an OpenAL function";
            break;
        case AL_INVALID_ENUM:
            s = "an invalid enum value was passed to an OpenAL function";
            break;
        case AL_INVALID_VALUE:
            s = "an invalid value was passed to an OpenAL function";
            break;
        case AL_INVALID_OPERATION: 
            s = "the requested operation is not valid";
            break;
        case AL_OUT_OF_MEMORY: 
            s = "the requested operation resulted in OpenAL running out of memory";
            break;
        default:
            s = "error not recognised";
    }
    fprintf(stderr, "OPENAL ERROR OCCURED! INFO: %s",s);
    return;
}

#define DEFAULT_CHANNEL_NO 16
#define NUM_BUFFERS 8
#define M_PI 3.14159265358979323846

//Source defaults
#define SOURCE_PITCH_DEFAULT 1
#define SOURCE_GAIN_DEFAULT  1.f
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
}   hsh_aSource;

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
 *  \return 0 on failure, non-zero on success
 * 
 *  \sa hsh_initSource
 * 
*/
extern int hsh_freeSource(hsh_aSource* hsh_src);

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
 *  \return number of buffers sucessfully written to, likely used to queue to the corresponding openAL source. negative on fatal error
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
 *  \return 0 on failure, non-zero on success
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
 *  \return 0 on failure, non-zero on success
 * 
 *  \sa hsh_playSound
*/
extern int hsh_playSoundFromFile(const char* file,hsh_aSource* src,int16_t loops, int32_t mtime,int channels);

/**
 * pauses playback at source, can be resumed with hsh_unpauseSource
 * 
 *  \param src audio source to pause playback at
 *  \return 0 on failure, non-zero on success
 * 
 *  \sa hsh_unpauseSource
*/
extern int hsh_pauseSource(hsh_aSource* src);

/**
 * resumes playback at source if paused by hsh_pauseSource, otherwise has no effect
 * 
 * \param src audio source to resume playback at
 * \return 0 on failure, non-zero on success
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
 *  \return number of new buffers just queued to be played at source. negative on fatal error
 *  
 *  \sa hsh_playSound
*/
extern int8_t hsh_feedSource(hsh_aSource* hsh_src);

/**
 * moves the spatial location of the source to the given position (as hsh_vec3)
 * 
 * \param hsh_src audio source to move to position
 * \param pos position to move audio source to
 * 
 * \return 0 on failure, non-zero on success
*/
extern int hsh_moveSource(hsh_aSource* hsh_src, hsh_vec3 pos);

/**
 * rotates the listener/player at its current position.
 * Takes two floating point angles (in degrees) pitch and yaw.
 * Follows a right hand coordinate system: the z axis points towards the screen, the x axis to the side and the y axis up).
 * Rotation is clockwise.
 * 
 * \param pitch the pitch value represents rotation along the x axis. Modelling with the head of the player
 * it would be equivalent to looking up and down. It is clockwise going down s.t a value of 90 will face the listener directly downwards
 * 
 * \param yaw the yaw value represents rotation along the y aixs. Modelling with the head of the player
 * it would be equivalient to looking left and right. It goes clockwise s.t a value of 90 will face the listener directly right looking at the negative x axis
 * 
 * \return 0 on failure, non-zero on success
 * 
 * \sa hsh_setListenerPosition
*/
extern int hsh_rotateListener(float pitch, float yaw);

/**
 * moves the listener/player to the given world position.
 * Takes a vector3 representing x,y,z coordinates.
 * where x and z represent directions right/left/forward/backwards and y represents up/down
 * 
 * \param pos postion to move to Vector3(x,y,z)
 * 
 * \return non-zero on sucess, 0 on failure
*/
extern int hsh_setListenerPos(hsh_vec3 pos);

/**
 * returns the current position the listener/player is located in.
 * 
 * \return the position of the listener/player. Will return a vector of {0,0,0}
 * if somehow a listener is not present.
*/
extern hsh_vec3 hsh_getListenerPos(void);

/** 
 * determines if a source is currently playing audio.
 * 
 * \param src audio source to check
 * \return nonzero if source is playing, 0 if source is not playing or source is not found. 
*/
extern int hsh_isSourcePlaying(hsh_aSource* src);

/**
 * intialises hush engine sample playbakc, must be called before using ANY sample functions
 * memory should be freed before the end of the program by call hsh_closeSamplePlayback().
 * This function is called by use hsh_init();
 * 
 * \param format audio format to conform all given samples to. Must be either AUDIO_S16 or AUDIO_S8
 * \param rate audio frequency to conform given samples to.
 * 
 * \return non-zero on success, 0 on error
 * 
 * \sa hsh_closeSamplePlayback
*/
extern int hsh_initSamplePlayback( Uint16 format,Uint32 rate);

/**
 * closes sample playback and memory created by hsh_initSamplePlayback();
 * This function is called by hsh_close();
 * 
 * \sa hsh_init()
*/
extern void hsh_closeSamplePlayback();

#endif

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define TABLE_CAPACITY 100
#define BUFFER_SIZE 65536

//Test for a hash table for allocated samples

typedef struct Entry {
    const char* file;
    Sound_Sample* sample;
    struct Entry* next;

} Entry;

typedef struct Table {
    Entry** entries;
    int capacity;
    int size;

} Table;

typedef struct hsh_AudioInfo{
	
	ALCdevice* device;
	Sound_AudioInfo* desired_Format;
    ALCcontext* context;
    int32_t bpms;

} hsh_AudioInfo;

Sound_AudioInfo* get_DesiredAudioInfo();
ALCdevice* get_AudioDevice();

hsh_AudioInfo *hush_AI;

Table* create_Table(void);

void delete_Table();

void free_Entry(Entry* e);

/*
Hashes filename and returns pointer to entry in hash table
    -Creates and entry is one doesnt exist.
    -Finds and entry is it does already exist.
    -Returns NULL if table hasnt been initialised, is at max capacity, or sample couldnt be loaded
*/
Entry* hash_lookup (const char* file,int channels);

/*
Simple Implementation of a djb2 hash function returns hash from given string.
*/
unsigned long djb2_hash(const char *str);

void print_ht();

//audio info


#endif

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
extern uint8_t hsh_enqueueSample(hsh_SampleQueue* sq,
                                 Sound_Sample* sample,
                                 hsh_aSource* hsh_src, 
                                 int16_t loops, 
                                 int32_t mtime);

extern uint8_t hsh_enqueueSampleFromFile(const char* file,
                                         hsh_SampleQueue* sq, 
                                         hsh_aSource* hsh_src,
                                         int16_t loops,
                                         int32_t mtime,
                                         int channels);

extern int8_t hsh_enqueueDelay(int32_t time, hsh_SampleQueue* sq);

extern uint8_t hsh_dequeueSample(hsh_SampleQueue* sq);



/*
Handles playing from a given SampleQueue
- Will play the next sample in the queue 
if the queue is not empty nothing the queue is playing is playing
- Then dequeue it
*/
int hsh_handleQueue(hsh_SampleQueue* sq);

void printf_Q(hsh_SampleQueue* sq);


#endif
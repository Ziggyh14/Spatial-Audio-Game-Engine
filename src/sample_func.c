#include "sample_func.h"

// Amount of queues initialised

hush_AudioSource* hush_init_Source(ALfloat pitch,ALfloat gain,hush_Vector3 position,hush_Vector3 velocity){

    hush_AudioSource* hsh_src =  malloc(sizeof(hush_AudioSource));
    alAssert(alGenSources(1,&(hsh_src->source)));
    alAssert(alSourcef(hsh_src->source,AL_PITCH,pitch));
    alAssert(alSourcef(hsh_src->source,AL_GAIN,gain));
    alAssert(alSource3f(hsh_src->source,AL_POSITION,position.x,position.y,position.z));
    alAssert(alSource3f(hsh_src->source,AL_VELOCITY,velocity.x,velocity.y,velocity.z));
    
    
    alAssert(alSourcei(hsh_src->source,AL_LOOPING,AL_FALSE));
    

    alAssert(alGenBuffers(NUM_BUFFERS,&(hsh_src->buffers)));
    printf("source created\n");
    return hsh_src;
}

extern int hush_playSoundAtSource(const char* file,hush_AudioSource* hsh_src,int16_t loops){

    int i;
    hsh_src->entry = hash_lookup(file);
    if(loops == -1)
        loops = INT16_MAX;
    
    hsh_src->entry->loops = loops;
    Sound_Sample* sample = hsh_src->entry->sample;
    for(i = 0;i<NUM_BUFFERS;i++){
        Uint32 size = Sound_Decode(sample);
        void* data = calloc(BUFFER_SIZE,1);
        memcpy(&data[0],sample->buffer,size);
        
        if(size<BUFFER_SIZE){
            hsh_src->entry->loops--;
            if(hsh_src->entry->loops>=0){
                Sound_Rewind(sample);
                Sound_SetBufferSize(sample, BUFFER_SIZE-size);
                Sound_Decode(sample);
                memcpy(&data[size],sample->buffer,BUFFER_SIZE-size);
                Sound_SetBufferSize(sample, BUFFER_SIZE);
            }
        }
        alAssert(alBufferData(hsh_src->buffers[i],AL_FORMAT_STEREO16,data,BUFFER_SIZE,sample->actual.rate));
        free(data);
    }    

    alAssert(alSourceQueueBuffers(hsh_src->source,NUM_BUFFERS,&(hsh_src->buffers[0])));
    alAssert(alSourcePlay(hsh_src->source));
    printf("sample playing\n");
    return 0;
}

extern void feed_source(hush_AudioSource* hsh_src){

    int buffersProcessed = 0;
    alAssert(alGetSourcei(hsh_src->source,AL_BUFFERS_PROCESSED,&buffersProcessed));

    if(buffersProcessed<=0)
        return;

    while(buffersProcessed-- && hsh_src->entry->loops>=0){

        ALuint buf;
        alAssert(alSourceUnqueueBuffers(hsh_src->source,1,&buf))

        void* data = calloc(BUFFER_SIZE,1);
        Sound_Sample* sample = hsh_src->entry->sample;
        Uint32 size = Sound_Decode(sample);
        memcpy(&data[0],sample->buffer,size);
        if(size<BUFFER_SIZE){
            hsh_src->entry->loops--;
            if(hsh_src->entry->loops>=0){

                Sound_Rewind(sample);
                Sound_SetBufferSize(sample, BUFFER_SIZE-size);
                Sound_Decode(sample);
                memcpy(&data[size],sample->buffer,BUFFER_SIZE-size);
                Sound_SetBufferSize(sample, BUFFER_SIZE);
            }
        }      

        alAssert(alBufferData(buf,AL_FORMAT_STEREO16,data,BUFFER_SIZE,sample->actual.rate));
        alAssert(alSourceQueueBuffers(hsh_src->source,1,&buf));
        free(data);
    }

}


void init_Sample_Playback(Uint16 format,Uint32 rate){ //todo make init variables parameters
    
    create_Table();
    Sound_Init();
    ALCdevice* d = alcOpenDevice(NULL);
    hush_AI = malloc(sizeof(hush_AudioInfo));
    hush_AI->device = d;
    hush_AI->desired_Format = malloc(sizeof(Sound_AudioInfo));
    hush_AI->desired_Format->channels = 2;
    hush_AI->desired_Format->format   = format;
    hush_AI->desired_Format->rate     = rate;
    queue_Count = 0;
    printf("playback initialised\n");

}

void close_Sample_Playback(){

    Sound_Quit();
    alcCloseDevice(get_AudioDevice());
    delete_Table(); //delete the table
}



#include "sample_func.h"
#include "math.h"


hsh_aSource* hsh_initSource(ALfloat pitch,ALfloat gain,hsh_vec3 position,hsh_vec3 velocity){

    hsh_aSource* hsh_src =  malloc(sizeof(hsh_aSource));
    alAssert(alGenSources(1,&(hsh_src->alSource)));
    alAssert(alSourcef(hsh_src->alSource,AL_PITCH,pitch));
    alAssert(alSourcef(hsh_src->alSource,AL_GAIN,gain));
    alAssert(alSource3f(hsh_src->alSource,AL_POSITION,position.x,position.y,position.z));
    alAssert(alSource3f(hsh_src->alSource,AL_VELOCITY,velocity.x,velocity.y,velocity.z));
    alSourcef(hsh_src->alSource, AL_REFERENCE_DISTANCE, SOURCE_REFERENCE_DEFAULT);
    alSourcef(hsh_src->alSource, AL_ROLLOFF_FACTOR, SOURCE_ROLLOFF_DEFAULT);

    alAssert(alSourcei(hsh_src->alSource,AL_LOOPING,AL_FALSE));
    hsh_src->buffers = (ALuint*) malloc(sizeof(ALuint)*NUM_BUFFERS);
    
    alAssert(alGenBuffers(NUM_BUFFERS,(hsh_src->buffers)));
    alAssert(alSourceStop(hsh_src->alSource));
    printf("source created\n");
    return hsh_src;
}

extern void hsh_freeSource(hsh_aSource* hsh_src){
    alAssert(alSourcei(hsh_src->alSource,AL_BUFFER,NULL));
    alAssert(alDeleteBuffers(NUM_BUFFERS,hsh_src->buffers));
    alAssert(alDeleteSources(1,hsh_src));
    free(hsh_src);
}

extern int hsh_playSound(Sound_Sample* sample,hsh_aSource* hsh_src,int16_t loops, int32_t mtime){

    if(loops == -1)
        loops = INT16_MAX;
    
    if(mtime == -1)
        mtime = INT32_MAX;

    hsh_src->mtime = mtime;
    hsh_src->loops = loops;
    hsh_src->sample = sample;

    Sound_Rewind(hsh_src->sample);
    alAssert(alSourcei(hsh_src->alSource,AL_BUFFER,NULL));

    ALuint i = hsh_bufferMath(hsh_src, hsh_src->buffers,hsh_src->sample, NUM_BUFFERS);
    
    alAssert(alSourceQueueBuffers(hsh_src->alSource,i,&(hsh_src->buffers[0])));
    alAssert(alSourcePlay(hsh_src->alSource));
    printf("sample playing\n");
    return 0;
}

ALuint hsh_bufferMath(hsh_aSource* hsh_src ,ALuint* buffers , Sound_Sample* sample, ALuint n){

    int i;

    for(i = 0;i<n;i++){

        if(hsh_src->mtime * hush_AI->bpms == 0)
            return i;

        Uint32 size = Sound_Decode(sample);

        const char* s = Sound_GetError();
        if (s)
            printf("%s\n",s);

        ALvoid* data = calloc(BUFFER_SIZE,1);

        printf("size: %d\n", size);

        if(size > (hsh_src->mtime * hush_AI->bpms)){
            memcpy(&data[0],sample->buffer,hsh_src->mtime * hush_AI->bpms);
            hsh_src->mtime = 0;
            alAssert(alBufferData(buffers[i],AL_FORMAT_MONO16,data,BUFFER_SIZE,sample->actual.rate));
            free(data);
            break;
        }

        hsh_src->mtime -= size / hush_AI->bpms;

        memcpy(&data[0],sample->buffer,size);
        if(size < BUFFER_SIZE){
            hsh_src->loops--;
            printf("loops: %d\n", hsh_src->loops);
            if(hsh_src->loops >= 0){

                Sound_SetBufferSize(sample, BUFFER_SIZE-size);
                if(Sound_Rewind(sample) == 0){
                    printf("rewind error\n");
                }
                Uint32 tsize = Sound_Decode(sample);
                while (!tsize){
                    Sound_Rewind(sample);
                    tsize = Sound_Decode(sample);
                }
                memcpy(&data[size],sample->buffer,tsize);
                hsh_src->mtime -= tsize / hush_AI->bpms;
                Sound_SetBufferSize(sample, BUFFER_SIZE);
                printf("samplesize: %d\n", sample->buffer_size);
            }else if(size == 0){
                free(data);
                return i;
            }
        }

        alAssert(alBufferData(buffers[i],AL_FORMAT_MONO16,data,BUFFER_SIZE,sample->actual.rate));
        free(data);
    }    

    return i;
}

extern int hsh_playSoundFromFile(const char* file,hsh_aSource* src,int16_t loops, int32_t mtime){
  Entry* e = hash_lookup(file);
  return hsh_playSound(e->sample,src,loops,mtime);
}

extern int hsh_pauseSource(hsh_aSource* src){

    alAssert(alSourcePause(src->alSource));
}

extern int hsh_unpauseSource(hsh_aSource* src){

    alAssert(alSourcePlay(src->alSource));
}

extern int8_t hsh_feedSource(hsh_aSource* hsh_src){

    int i;
    int buffersProcessed = 0;
    alAssert(alGetSourcei(hsh_src->alSource,AL_BUFFERS_PROCESSED,&buffersProcessed));

    if(buffersProcessed<=0)
        return 0;

    while(buffersProcessed--){
        ALuint buf;
        alAssert(alSourceUnqueueBuffers(hsh_src->alSource, 1, &buf));
        i = hsh_bufferMath(hsh_src, &buf, hsh_src->sample, 1);
        alAssert(alSourceQueueBuffers(hsh_src->alSource,i,&buf));
    }

    return i;
}

extern int hsh_moveSource(hsh_aSource* hsh_src, hsh_vec3 pos){
    alAssert(alSource3f(hsh_src->alSource, AL_POSITION, pos.x,pos.y,pos.z));
}

extern int hsh_rotateListener(float pitch,float yaw){

    float or[] = {0.,0.,1.,0.,1.,0.};

    //convert to radians

    pitch = (pitch) * (M_PI/180);
    yaw   = (yaw) * (M_PI/180);

    // FORWARD / AT VECTOR
    or[0] = (sin(yaw)*cos(pitch));
    or[1] = -sin(pitch);
    or[2] = (cos(yaw) * cos(pitch));

    //UP VECTOR

    or[3] = sin(yaw) * sin(pitch);
    or[4] = cos(pitch);
    or[5] = cos(yaw) * sin(pitch);


    alAssert(alListenerfv(AL_ORIENTATION,or));
}

void hsh_initSamplePlayback(Uint16 format,Uint32 rate){ //todo make init variables parameters
    
    create_Table();
    Sound_Init();
    
    ALCdevice* d = alcOpenDevice(NULL);
    hush_AI = malloc(sizeof(hush_AudioInfo));
    hush_AI->device = d;
    hush_AI->desired_Format = malloc(sizeof(Sound_AudioInfo));
    hush_AI->desired_Format->channels = 1;
    hush_AI->desired_Format->format   = format;
    hush_AI->desired_Format->rate     = rate;
    if(hush_AI->desired_Format->format == AUDIO_S16){
        // calculate in bytes per milisecond of samples
        hush_AI->bpms = (((16 * hush_AI->desired_Format->channels * hush_AI->desired_Format->rate) / 8) / 1000);
    }
    if(hush_AI->desired_Format->format == AUDIO_S8){
        // calculate in bytes per milisecond of samples
        hush_AI->bpms = (((8 * hush_AI->desired_Format->channels * hush_AI->desired_Format->rate) / 8) / 1000);
    }

    ALCint attrs[] = {ALC_HRTF_SOFT,ALC_TRUE,0};
    ALCcontext* c = alcCreateContext(get_AudioDevice(),attrs);
    alcMakeContextCurrent(c);

    printf("playback initialised\n");

}

void hsh_closeSamplePlayback(){

    Sound_Quit();
    alcCloseDevice(get_AudioDevice());
    delete_Table(); //delete the table
}




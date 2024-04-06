/*
#include "sdl_func.h"
#include "sample_func.h"
#include "state_manager.h"
#include "event_handler.h"
#include "header.h"

#define ui64 uint64_t
#define ui32 uint32_t

#define PLAYER_VEL 0.5
#define TRUE 1
#define FALSE 0

#define QUIT_CHECK if(SDL_QuitRequested()){break;}

float playerpos = 0;
int delay = 0;

ui32 update1(ui32 dT){

    SDL_Event e;
    while(pollEvent(&e)){
        if(isKeyDown(e)){
            switch(getKeyPressed(e)){
                case KEY_UP: playerpos = playerpos + (PLAYER_VEL * dT); break;
                case KEY_DOWN: playerpos = playerpos - (PLAYER_VEL * dT); break;
                default: break;
            }
        }
    }
    printf("%f\n",playerpos);
    return 0;
}

ui32 draw1(ui32 dT){
    
    if(playerpos == 50 && delayPassed(&delay)){
        setDelay(&delay,1000);
        //play_Sample("bin/jazz.wav",0);
    }
    return 0;
}

ui32 init1(){

    play_Sample("bin/goblin.wav",0);
    return 0;
}

ui32 init2(){

    play_Sample("bin/chaching.wav",0);
    return 0;
}

ui32 destroy(){
    return 0;
}

int main(int argc, char** argv){

    State* s = calloc(1,sizeof(State));
    s->draw = &draw1;
    s->update = &update1;
    s->init = &init1;
    s->destroy = &destroy;

    State* s1 = calloc(1,sizeof(State));
    s1->draw = &draw1;
    s1->init = &init2;
    s1->destroy = &destroy;

    StateManager* sa = init_StateManager(5);
    create_Table(); // must be called before init_window

    SDL_Window* w = init_Window("DEMO", 1280,720); //init sdl and create a window

    init_Sample_Playback(96000,MIX_DEFAULT_FORMAT,2048); //init sdl mixer, and reserve space for space queue

    sample_Volume("bin/gibbon.wav",10);
    play_Sample("bin/gibbon.wav",0); //play a sample from file, play loop+1 times

    SampleQueue* q = init_Queue();
    enqueue_Sample("bin/brick.wav",-1,q);
    enqueue_Delay(500,q);
    enqueue_Sample("bin/brick.wav",-1,q);


    ui32 lastTick = 0; // stores the last time for dT calc 
    ui32 dT = 0; // variable for delta time used for physics calculations

    push_State(s,sa);
    printf("hello\n");

    while (TRUE){ // game loop to run indefinitely unless quit

        QUIT_CHECK; // alias for detecting if a quit event is queued and breaks if there is

        dT = SDL_GetTicks() - lastTick; // dT is time passed between now and completion of last frame
        lastTick = SDL_GetTicks(); // set last tick for next loop

        handle_Queue(q);
        //STATE FUNCTIONS
        update_StateManager(sa,dT);
       // s.draw(dT); // "draw" anything that needs drawing
    }
    close_Sample_Playback(); // close sample playback and delete the hash table
    close_Window(w); // close window and exit SDL
}


*/
/*
#include <stdio.h>
#include "SDL.h"
#include "AL/al.h"
#include "AL/alc.h"

int main(int argc, char **argv){
    printf("hello world!\n");
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    ALCdevice *device = alcOpenDevice(NULL);
}
*/
/*
#include "SDL_sound.h"
#include "al.h"
#include "alc.h"
#include "alext.h"


int main(){
    
    Sound_Init();
    Sound_Sample* s;
    Sound_AudioInfo* i = malloc(sizeof(Sound_AudioInfo));
   
    i->channels = 2;
    i->rate = 44100;
    i->format = AUDIO_S16;

    s = Sound_NewSampleFromFile("jazz.wav",i,65536);

    if(s==NULL){
        printf("no s\n");
    }

    Sound_DecodeAll(s);
   
    printf("channels, %d\nfreq: %d,\nbitspersample: %x\nsize: %d\nflags: %x\n",
            s->actual.channels,s->actual.rate,s->actual.format,s->buffer_size,s->flags);
    
    ALCdevice* d = alcOpenDevice(NULL);
    ALCcontext* c = alcCreateContext(d,NULL);
    alcMakeContextCurrent(c);
    printf("device open and context made\n");

    ALuint buffer;
    alGenBuffers(1,&buffer);
    ALenum f = AL_FORMAT_STEREO16;
    alBufferData(buffer,f,s->buffer,s->buffer_size,s->actual.rate);
    printf("buffer generated\n");

    ALuint sc;
    alGenSources(1,&sc);
    alSourcei(sc,AL_LOOPING, AL_FALSE);
    alSourcei(sc,AL_BUFFER,buffer);
    printf("source added\n");

    alSourcePlay(sc);
    ALint state = AL_PLAYING;

    while(state == AL_PLAYING)
    {
        //printf("sound playing\n");
        alGetSourcei( sc, AL_SOURCE_STATE, &state);
    }
    
    Sound_FreeSample(s);
    Sound_Quit();
    alcDestroyContext(c);
    alcCloseDevice(d);



}
*/

#include "sample_func.h"
#include "math.h"
#include "stdio.h"
#include "time.h"

#define QUIT_CHECK if(SDL_QuitRequested()){break;}


//#define M_PI                         (3.14159265358979323846)



int main(){

    SDL_Init(SDL_INIT_AUDIO);
    hsh_initSamplePlayback(AUDIO_S16,44100);

    ALCint attrs[] = {ALC_HRTF_SOFT,ALC_TRUE,0};

    ALCcontext* c = alcCreateContext(get_AudioDevice(),attrs);
    alcMakeContextCurrent(c);

    hsh_vec3 pos = {0,0,0};
    hsh_vec3 player = {0,0,0};
    hsh_vec3 vel = {0,0,0};
    alListener3f(AL_POSITION,player.x,player.y,player.z);
    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
    printf("x:%f,y:%f,z:%f\n",pos.x,pos.y,pos.z);
    hsh_aSource* src = hsh_initSource(1.f,1.f,pos,vel);


    ALint hrtf_state;
    ALint state;
        /* Check if HRTF is enabled, and show which is being used. */
    alcGetIntegerv(get_AudioDevice(), ALC_HRTF_SOFT, 1, &hrtf_state);
    if(!hrtf_state)
        printf("HRTF not enabled!\n");
    else{
        const ALchar *name = alcGetString(get_AudioDevice(), ALC_HRTF_SPECIFIER_SOFT);
        printf("HRTF enabled, using %s\n", name);
    }
    
    hsh_SampleQueue* q = hsh_initQueue();
    hsh_enqueueSampleFromFile("door.wav",q,-1,-1,src);
    hsh_enqueueSampleFromFile("jazz.wav",q,-1,0,src);

    int i = 100;
    while(i > 0){
        i -= 10;
        QUIT_CHECK;
        hsh_feedSource(src);
        alGetSourcei(src->alSource,AL_SOURCE_STATE,&state);
        switch(state){
            case AL_PLAYING:
                //printf("playing\n");
                break;
            case AL_INITIAL:
                printf("initial\n");
                break;
            case AL_STOPPED:
                printf("stopped\n");
                return 0;
                break;
        }
        SDL_Delay(10);
    }
    while(1){
        QUIT_CHECK;
        hsh_handleQueue(q);
        SDL_Delay(10);
    }

    alcDestroyContext(c);
    hsh_closeSamplePlayback();
    SDL_Quit();

}


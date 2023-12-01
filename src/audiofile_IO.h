#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>



#define LITTLE_E 1
#define BIG_E 0

typedef struct AudioChunk{
    //header
    uint16_t channels;
    uint32_t sampleRate;
    uint16_t bitsPerSample;
    uint32_t size;
    //data
    void* data;
}AudioChunk;

uint32_t toInt(char* buffer,size_t len,int endianness){
    
    int32_t result = 0;

    if(len == 2){
        if (endianness == LITTLE_E)
            result = (buffer[1] << 8) | buffer[0];
        else
            result = (buffer[0] << 8) | (buffer[1]); 
    }
    if(len == 4){
        if (endianness == LITTLE_E)
            result = (buffer[3] << 24) | (buffer[2] << 16)| (buffer[1] << 8) | buffer[0];
        else
            result = (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
    }
    return result;
    
}

AudioChunk* load_wav(const char* filename){

    FILE* file = fopen(filename,"r");
    AudioChunk* ac = malloc(sizeof(AudioChunk));
    if(file == NULL){
        printf("NO FILE ERROR\n");
        return NULL;
    }

    //RIFF

    char buff[4];
    if(!fread(buff,4,1,file)){
        printf("ERROR: NO RIFF HEADER FOUND\n");
        return NULL;
    }
    if(toInt(buff,4,BIG_E) != 0x52494646){
        printf("ERROR: RIFF NOT FOUND IN FILE\n");
        return NULL;
    }
    if(!fread(buff,4,1,file)){
        printf("ERROR: NO Chunksize FOUND\n");
        return NULL;
    }
    if(!fread(buff,4,1,file)){
        printf("ERROR: NO WAVE HEADER FOUND\n");
        return NULL;
    }
    if(toInt(buff,4,BIG_E) != 0x57415645){
        printf("ERROR: WAVE NOT FOUND IN FILE\n");
        return NULL;
    }

    //fmt 

    if(!fread(buff,4,1,file)){
        printf("ERROR: NO FMT HEADER FOUND\n");
        return NULL;
    }
    if(!fread(buff,4,1,file)){
        printf("ERROR: NO 16 PMT HEADER FOUND\n");
        return NULL;
    }
    if(!fread(buff,2,1,file)){
        printf("ERROR: NO PCM HEADER FOUND\n");
        return NULL;
    }
    if(!fread(buff,2,1,file)){
        printf("ERROR: NO CHANNELS FOUND\n");
        return NULL;
    }

    ac->channels = (uint16_t) toInt(buff,2,LITTLE_E);
    printf("channels: %u\n",ac->channels);

    if(!fread(buff,4,1,file)){
        printf("ERROR: NO SAMPLERATE FOUND\n");
        return NULL;
    }
    ac->sampleRate = toInt(buff,4,LITTLE_E);
    printf("freq: %d \n",ac->sampleRate);

    if(!fread(buff,4,1,file)){
        printf("ERROR: NO BYTERATE FOUND\n");
        return NULL;
    }
    if(!fread(buff,2,1,file)){
        printf("ERROR: NO BLOCKALIGN FOUND\n");
        return NULL;
    }
    if(!fread(buff,2,1,file)){
        printf("ERROR: NO BITS PER SAMPLE FOUND\n");
        return NULL;
    }
    ac->bitsPerSample = (uint16_t) toInt(buff,2,LITTLE_E);
    printf("bits per sample: %u\n",ac->bitsPerSample);

    //data

    if(!fread(buff,4,1,file)){
        printf("ERROR: NO DATA HEADER FOUND\n");
        return NULL;
    }
    if(toInt(buff,4,BIG_E) != 0x64617461){ //big endian for 'data'
        printf("ERROR: DATA NOT FOUND IN FILE\n");
        return NULL;
    }
    if(!fread(buff,4,1,file)){
        printf("ERROR: NO DATA SIZE HEADER FOUND\n");
        return NULL;
    }
    ac->size = toInt(buff,4,LITTLE_E);
    printf("data size: %d\n",ac->size);
    return NULL;

}


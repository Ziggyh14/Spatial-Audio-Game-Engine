
#include "al.h"
#include "alc.h"
#include "alext.h"
#include "SDL_sound.h"

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define TABLE_CAPACITY 50
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

typedef struct hush_AudioInfo{
	
	ALCdevice* device;
	Sound_AudioInfo* desired_Format;
    int32_t bpms;

} hush_AudioInfo;

Sound_AudioInfo* get_DesiredAudioInfo();
ALCdevice* get_AudioDevice();

hush_AudioInfo *hush_AI;

Table* create_Table(void);

void delete_Table();

void free_Entry(Entry* e);

/*
Hashes filename and returns pointer to entry in hash table
    -Creates and entry is one doesnt exist.
    -Finds and entry is it does already exist.
    -Returns NULL if table hasnt been initialised, is at max capacity, or sample couldnt be loaded
*/
Entry* hash_lookup (const char* file);

/*
Simple Implementation of a djb2 hash function returns hash from given string.
*/
unsigned long djb2_hash(const char *str);

void print_ht();

//audio info


#endif

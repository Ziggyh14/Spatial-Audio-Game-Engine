
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define TABLE_CAPACITY 50

#endif
//Test for a hash table for allocated samples

typedef struct Entry {
    const char* file;
    Mix_Chunk* chunk;
    struct Entry* next;

} Entry;

typedef struct Table {
    Entry** entries;
    int capacity;
    int length;

} Table;


Table* create_Table(void);

void delete_Table(Table* t);

void free_Entry(Entry* e);

/*
Hashes filename and returns pointer to entry in hash table
    -Creates and entry is one doesnt exist.
    -Finds and entry is it does already exist.
    -Returns NULL if table is at capacity.
*/
Entry* hash_lookup (const char* file, Table* table);

/*
Simple Implementation of a djb2 hash function returns hash from given string.
*/
unsigned long djb2_hash(const char *str);




#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "sample_hash_table.h"

#ifndef SAMPLE_FUNC_H
#define SAMPLE_FUNC_H

#define SD_CHANNEL_GROUP 1
#define SD_CHANNELS_LEN 5

#endif

void init_Sample_Playback();

/*
- Plays a sample from a given file,
- If it has not already been played it will allocate memory
- If it has already been used it will NOT reallocate new memory
- RETURNS 0 on success, 1 on failure
*/
int play_Sample(const char* file);


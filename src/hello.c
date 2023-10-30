#include "hello.h"
#include "sample_hash_table.h"

SDL_Window* init_Window (const char* title, int width, int height){

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)){
        printf("error initializing SDL: %s\n", SDL_GetError());
        return NULL;
        }

    return SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED_DISPLAY(0),
            SDL_WINDOWPOS_CENTERED_DISPLAY(0),
            width,
            height,
            SDL_WINDOW_ALLOW_HIGHDPI);
}

void close_Window(SDL_Window* window){

    SDL_DestroyWindow(window);
    SDL_Quit();
}

Mix_Chunk* play_Sample(const char* file){
    Mix_Chunk* temp = Mix_LoadWAV(file);
    Mix_PlayChannel(-1 ,temp,0);
    return temp;
}


int main(int argc, char** argv){
    int i;
    Table* ht = create_Table();
    if (ht == NULL){
        printf("ERROR: NO TABLE MADE\n");
        return 1;
    }

    printf("%s",hash_lookup("hello.wav",ht)->file);
    hash_lookup("hello",ht);
    hash_lookup("yorpgorgr",ht);

    //printf the hash table
    printf("\n TABLE, CAPACITY = %d\n",ht->capacity);
    for(i=0;i<TABLE_CAPACITY;i++){
        if(ht->entries[i] == NULL){
            printf("%d:-----\n",i);
        }else{
            printf("%d: ",i);
            Entry* e = ht->entries[i];
            printf("%s, ",e->file);
            while(e->next != NULL){
                e=e->next;
                printf("%s, ",e->file);
            }
            
            printf("\n");
        }
    }
    delete_Table(ht); //delete the table
}
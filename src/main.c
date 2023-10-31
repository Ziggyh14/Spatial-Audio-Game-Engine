#include "sdl_func.h"
#include "sample_func.h"

int main(int argc, char** argv){
    int i;
    create_Table();
    if (ht == NULL){
        printf("ERROR: NO TABLE MADE\n");
        return 1;
    }

    SDL_Window* window = init_Window("DEMO", 1280,720);
    
    //Mixer test
    init_Sample_Playback();
    
   
    play_Sample("bin/bassloop.wav",0);

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
    
    SDL_Event e;
    int quit = 0;
    while(!quit){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                quit = 1;
            }
            if(e.type == SDL_KEYDOWN){
                
            }
        }
    }
    Mix_CloseAudio();
    delete_Table(); //delete the table

    close_Window(window);
}
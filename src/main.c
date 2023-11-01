#include "sdl_func.h"
#include "sample_func.h"

void print_ht();

int main(int argc, char** argv){

    create_Table();
    if (ht == NULL){
        return 1;
    }

    
    SDL_Window* window = init_Window("DEMO", 1280,720);
    
    init_Sample_Playback();
    printf("%d\n",queue_Count);
    SampleQueue* q = init_Queue();

    printf_Q(q);

    enqueue_Sample("bin/jazz.wav",-1,q);
   
    enqueue_Sample("bin/door.wav",-1,q);
    enqueue_Sample("bin/jazz.wav",-1,q);

    play_Sample("bin/bassloop.wav",0);

    print_ht();
    
    SDL_Event e;
    int quit = 0;
    while(!quit){
        handle_Queue(q);
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                quit = 1;
            }
            if(e.type == SDL_KEYDOWN){
                
            }
        }
    }

    close_Sample_Playback();

    close_Window(window);
}

/*
Print hash table
*/
void print_ht(){
    int i;
    if(ht==NULL)
        return;

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
}
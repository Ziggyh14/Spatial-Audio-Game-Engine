#include "sdl_func.h"
#include "sample_hash_table.h"

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
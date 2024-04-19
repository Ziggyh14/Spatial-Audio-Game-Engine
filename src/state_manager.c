#include "state_manager.h"

StateManager* hsh_initStateManager(int capacity){

    StateManager* sm = (StateManager*) malloc(sizeof(StateManager));
    if(sm == NULL ){
        printf("ERROR: STATE MANAGER NOT CREATED\n");
        return NULL;
    }
    sm->max = capacity;
    sm->top = -1;
    sm->stack = (State **) calloc(sm->max, sizeof(State*));
    if(sm->stack == NULL){
        free(sm);
        printf("ERROR: Could not create stack\n");
        return NULL;
    }
    return sm;
}
void hsh_freeStateManager(StateManager* sm){
    
    while(sm->top >= 0){
        hsh_popState(sm);
    }
    free(sm);
    return;
}
unsigned int hsh_pushState(State* s, StateManager* sm){

    if(sm->top == sm->max){
        printf("State stack at capacity, state cannot be added\n");
        return -1;
    }
    
    sm->stack[++(sm->top)] = s;
    printf("state added\n");
    return (hsh_peakState(sm))->init();

}
unsigned int hsh_popState(StateManager* sm){

    unsigned int r;
    if(sm->top<0){
        printf("State is empty cannot pop\n");
        return -1;
    }
    State* s = sm->stack[(sm->top)--];
    r = s->destroy();
    free(s);
    return r;
}
State* hsh_peakState(StateManager* sm){

    if(sm->top<0){
        printf("nothing in stack cannt peak\n");
        return NULL;
    }
    return sm->stack[sm->top];

}
unsigned int hsh_updateStateManager(StateManager* sm, unsigned int dT){
    
    if(sm->top<0){
        printf("State is empty\n");
        return -1;
    }
    return sm->stack[sm->top]->update(dT);
}
unsigned int hsh_drawStateManager(StateManager* sm, unsigned int dT){
    
    if(sm->top<0){
        printf("State is empty\n");
        return -1;
    }
    return sm->stack[sm->top]->draw(dT);
}


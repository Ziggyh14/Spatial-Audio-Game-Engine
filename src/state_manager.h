#include <stdio.h>
#include <stdlib.h>
#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

//typedef for function pointer and function pointer with float parameter
typedef unsigned int (*fp)();
typedef unsigned int (*fpFl)(float);

typedef struct State{
   
    unsigned int (*init)();
    unsigned int (*update)(unsigned int);
    unsigned int (*draw)(unsigned int);
    unsigned int (*destroy)();

} State;

typedef struct StateManager{

    State** stack;
    int max;
    int top;

} StateManager;

StateManager* init_StateManager(int capacity);
void free_StateManager(StateManager* sm);
unsigned int push_State(State* s, StateManager* sm);
unsigned int pop_State(StateManager* sm);
State* peak_State(StateManager* sm);

unsigned int update_StateManager(StateManager* sm, unsigned int dT);
unsigned int draw_StateManager(StateManager* sm, unsigned int dT);


#endif
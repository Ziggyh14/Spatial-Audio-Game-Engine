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

/**
 * Create new state manager return a pointer to it
 * return null on error
*/
StateManager* init_StateManager(int capacity);
/**
 * Free statemanager and contents of stack
*/
void free_StateManager(StateManager* sm);
/**
 * Push a state to the stack,
 *  returns result of state's init() function, -1 on error
*/
unsigned int push_State(State* s, StateManager* sm);
/**
 * Pop state from stack and free it
 * Return result of that states destroy() function, -1 on error
*/
unsigned int pop_State(StateManager* sm);
/**
 * Peak at the state on the stop of the stack
 * returns pointer to that state
*/
State* peak_State(StateManager* sm);
/**
 * Calls the update function of the state at the top of the stack
 * returns the result of that function, -1 on error
*/
unsigned int update_StateManager(StateManager* sm, unsigned int dT);
/**
 * Calls the draw function of the state at the top of the stack
 * returns the result of that function, -1 on error
*/
unsigned int draw_StateManager(StateManager* sm, unsigned int dT);


#endif
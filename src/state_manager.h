#include <stdio.h>
#include <stdlib.h>
#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H


typedef struct State{
   
    unsigned int (*init)();
    unsigned int (*update)(unsigned int);
    unsigned int (*draw)(unsigned int);
    unsigned int (*destroy)();

} State;

unsigned int hsh_defInit(){
    return 1;
}

unsigned int hsh_defDestroy(){
    return 1;
}

unsigned int hsh_defUpdate(unsigned int dT){
    return 1;
}

unsigned int hsh_defDraw(unsigned int dT){
    return 1;
}

State hsh_defaultState = {

    &hsh_defInit,
    &hsh_defUpdate,
    &hsh_defDraw,
    &hsh_defDestroy
};


typedef struct StateManager{

    State** stack;
    int max;
    int top;

} StateManager;

/**
 * Create new state manager return a pointer to it
 * return null on error
*/
StateManager* hsh_initStateManager(int capacity);
/**
 * Free statemanager and contents of stack
*/
void hsh_freeStateManager(StateManager* sm);
/**
 * Push a state to the stack,
 *  returns result of state's init() function, -1 on error
*/
unsigned int hsh_pushState(State* s, StateManager* sm);
/**
 * Pop state from stack and free it
 * Return result of that states destroy() function, -1 on error
*/
unsigned int hsh_popState(StateManager* sm);
/**
 * Peak at the state on the stop of the stack
 * returns pointer to that state
*/
State* hsh_peakState(StateManager* sm);
/**
 * Calls the update function of the state at the top of the stack
 * returns the result of that function, -1 on error
*/
unsigned int hsh_updateStateManager(StateManager* sm, unsigned int dT);
/**
 * Calls the draw function of the state at the top of the stack
 * returns the result of that function, -1 on error
*/
unsigned int hsh_drawStateManager(StateManager* sm, unsigned int dT);


#endif
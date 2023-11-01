#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

//typedef for function pointer and function pointer with float parameter
typedef unsigned int (*fp)();
typedef unsigned int (*fpFl)(float);

typedef struct State{
   
    unsigned int (*init)();
    unsigned int (*update)(float);
    unsigned int (*draw)(float);
    unsigned int (*free)();

} State;

typedef struct StateManager{

    State** stack;
    int max;
    int top;

} StateManager;

/*---------TODO----------
- init state manager
- free statemanger
- push
- pop
- head
- update
- draw
*/

#endif
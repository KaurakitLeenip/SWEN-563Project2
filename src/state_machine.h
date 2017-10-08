#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

/*
 * Implementing this using function pointers.  Will allow us to have it automatically
 * handle state switching through the functions, reducing overhead
*/ 
typedef void *(*StateFunc)();

// States: Put all states here as a function then implement their logic
// in the function

//************************* FILL MORE IN **************************************
// Servo states
void *Servo_At_0();
void *Servo_At_1();
void *Servo_At_2();
void *Servo_Init();
void *Servo_Unknown();

// program states
void *Post_State();
void *Running_State();

#endif
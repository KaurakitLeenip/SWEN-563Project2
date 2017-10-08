#include <stdio>
#include "state_machine.h"

void *Servo_At_0(){
    // some implementation
}

void *Servo_At_1(){
    // some implementation
}

void *Servo_At_2(){
    // some implementation
}

void *Servo_Unknown(){
    // some implementation
}

void *Servo_Init(){
    // some implementation
}

void *Post_State(){
    // some implementation
}

void *Running_State(){
    // some implementation
}

/* STATE IMPLEMENTATION EXAMPLE

void *Some_State(){
    do_things...
    
    return Some_Other_State; // next state
}

// showing off using an if/else
void *Some_Other_State(){
    do things...
    
    if(X){
        return Some_Other_Other_State;
    }
    else if(Y){
        return This_Weird_State;
    }
    else{
        return State_Unknown;
    }
}

// showing off using a switch statement
void *Some_Other_Other_State()
{
    do things...
    
    StateFunc nextState;
    switch(X){
        case some_case :
            nextState = Some_State;
            break; // optional
        case some_case_2 :
            nextState = Some_Other_state;
            break; // optional
        default: do something //optional
        nextState = Some_Other_Other_State;
        
    }
    return nextState;
            
/* USAGE EXAMPLE:

int main(){
    StateFunc programState = Post_State; // sets the state function to the inital state; post state and servo init
    StateFunc servoState = Servo_Init;
    
    // this will cycle through each state by calling the state itself.  Essentially all logic and switching
    // will be implemented in the function itself
    while(1){
        programState = (StateFunc)(*programState)();
        servoState = (StateFunc)(*servoState)();
    }
    
    return 1; // should never get here
}
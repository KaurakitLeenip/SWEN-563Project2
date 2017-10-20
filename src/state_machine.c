/**#include <stdio.h>
#include "state_machine.h"

int override_flag = 0;
char line[256];

typedef struct servo_info{
	
	enum status servo_status;
	enum servo_states servo_state;
	
	int current_index;
	int loop_start_index;
	int loop_counter;
	int servo_position_number;
	
}	servo;

enum status 
{
	status_input_checked,
	status_input_read,
	status_running,
	status_paused,
	status_command_error,
	status_nested_error
} ;

// This is a good way to define the state of a servo motor.
// This should be in a header (.h) file.
enum servo_states
{
	state_stopped,
	state_moving,
	status_recipe_ended		
	
} ;


**/
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
*/
            
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
*/
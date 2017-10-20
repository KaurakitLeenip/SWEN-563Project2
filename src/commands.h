#ifndef COMMANDS_H
#define COMMANDS_H

#include "commands.h"
#include "Timer.h"
#include "UART.h"
#include "LED.h"
#include <pthread.h>

#define MOV (0x20)
#define WAIT (0x40)
#define LOOP (0x80)
#define END_LOOP (0xA0)
#define RECIPE_END (0)
#define STEP_INTERVAL (320)
#define SMALLEST_WIDTH (400)
#define DEFAULT_STATE (3)

typedef struct{
	enum servo_states servo_state;
	
	int current_index;
	int loop_start_index;
	int loop_counter;
	int position;
	
}	servo;

enum servo_states
{
		
	state_paused,
	state_running,
	state_process_override,
	state_command_error,
	state_nested_error,
	state_recipe_end
	
}

enum status 
{
	status_input_read,
	status_running,
	status_done;
		
} ;

void Move_Buffering( int moves );
void * overrideCheck();
void Run_State();
void process_recipe( int index_number, int servo );
void override_process( char input, int servo );
void Turn_Left( int servo );
void Turn_Right( int servo )


#endif /* COMMANDS_H */

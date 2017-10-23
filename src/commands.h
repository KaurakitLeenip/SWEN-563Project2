#ifndef COMMANDS_H
#define COMMANDS_H

#include "commands.h"
#include "Timer.h"
#include "UART.h"
#include "LED.h"
#include "stm32l476xx.h"
#include <stdlib.h>

#define MOV (0x20)
#define WAIT (0x40)
#define LOOP (0x80)
#define END_LOOP (0xA0)
#define RECIPE_END (0)
#define STEP_INTERVAL (320)
#define SMALLEST_WIDTH (400)
#define DEFAULT_STATE (3)
#define NUM_SERVOS (2)

enum servo_states
{
		
	state_paused,
	state_running,
	state_process_override,
	state_command_error,
	state_nested_error,
	state_recipe_end
	
} ;

enum status 
{
	status_input_read,
	status_running,
	status_done
		
} ;

void Init_Servos( void );
void Move_Buffering( int moves );
void * overrideCheck( void );
void Run_State( void );
void process_recipe( int index_number, int servo );
void override_process( char input, int servo );
void Turn_Left( int servo );
void Turn_Right( int servo );


#endif /* COMMANDS_H */

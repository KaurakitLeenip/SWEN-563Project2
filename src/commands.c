#include "commands.h"

typedef struct{
	enum servo_states servo_state;
	
	int current_index;
	int loop_start_index;
	int loop_counter;
	int times_to_loop;
	int position;
	
}	servo;

int servo_number;
int override_flag = 0;
enum status program_status = status_running;
char line[256];
char rxByte;
servo servos[2];

char recipe1[100] = 
{
	MOV | 0, 
	MOV | 5, 
	MOV | 0,
	MOV | 3,
	LOOP | 0, 
	MOV | 1,
	MOV | 4,
	END_LOOP,
	MOV | 0,
	MOV | 2,
	WAIT | 0,
	MOV | 3,  
	WAIT | 0,
	MOV | 2,
	MOV | 3, 
	WAIT | 31, 
	WAIT | 31,
	WAIT | 31,
	MOV | 4,
	RECIPE_END

};

void Init_Servos(){
	int i;
	for ( i = 0; i < NUM_SERVOS; i++ ){
		servos[i].servo_state = state_running;
		servos[i].current_index = 0;
		servos[i].loop_counter = 0; 
		servos[i].position = 3;
	}
}

void Move_Buffering( int moves ){
	
	int delay = moves * 1000000;
	USART_Delay(delay);
	
}

/*void * overrideCheck(){
	int i;
	while(1){
		Read_Line(line);
		for ( i = 0; i < sizeof(line); i++ ){
			if ( line[i] == '>' ){
				override_flag = 1;
			}
		}
	}
}*/

void Run_State(){
	
	//CreateThread( NULL, NULL, overrideCheck, NULL );
	while( program_status != status_done ){
		int i;
		
		if ( USART2->ISR & 0x20 == 0x20 ){
			//if the RXNE interrupt has been flagged
			override_flag = 1;
		}
		
		if ( override_flag == 1 ){
			program_status = status_input_read;
			Read_Line(line);
		}
		
		 switch( program_status ){
			
			case status_input_read:
				
				Write_Line("\r\n>");
				for ( i = 0; i < sizeof(line); i++ ){
					if (line[i] == 'x' || line[i] == 'X'){
						servos[0].servo_state = state_running;
						servos[1].servo_state = state_running;
						override_flag = 0;
						
					}
					else{
						servos[0].servo_state = state_process_override;
						servos[1].servo_state = state_process_override;					
					}
				}
				override_flag = 0;
				

			case status_running:
				
				Red_LED_Off();
				Green_LED_Off();
				Green_LED_On();
			
				for ( servo_number = 0; servo_number < 2; servo_number++ ){
					if ( servos[servo_number].servo_state == state_running ){
						process_recipe( servos[servo_number].current_index, servo_number );
						servos[servo_number].current_index++;
					}
					else if (servos[servo_number].servo_state == state_process_override){
						override_process( line[servo_number], servo_number );
					}
					else if (servos[servo_number].servo_state == state_recipe_end){
						program_status = status_done;
					}
					else{
						//error and pause states do nothing
					}
				}
			case status_done:
				break;

		}
	}
}

void process_recipe( int index_number, int servo ){
	
	unsigned char temp;
	int in_loop = 0;
	int value;
	
	temp = recipe1[servos[servo].current_index];

	
	if ( (temp & MOV) == MOV ){
				
		int pulse_width;
		int delay_cycles;
				
		temp &= 0x1F;
		value = temp;
		pulse_width = SMALLEST_WIDTH + value*STEP_INTERVAL;
		Change_Width( pulse_width, servo );
		delay_cycles = abs(servos[servo].position - value);
		Move_Buffering(delay_cycles);
				
	}
			
	else if ( (temp & WAIT) == WAIT ){
		
		int delay;
		temp &= 0x1F;
		value = temp;

		value++;																
		//wait 0 is a wait of 1 cycle so the value must be adjusted
				
		delay = value*100000;
		USART_Delay(delay);
				
		}
			
	else if ( (temp & LOOP) == LOOP ){
		//save current index
		temp &= 0x1F;
		servos[servo].times_to_loop = temp;
		servos[servo].loop_start_index = index_number;
		servos[servo].loop_start_index++; 							
		//the next statements will be looped
				
		if( in_loop != 0 ){
		//NESTED_LOOP_ERROR: BOTH RED AND GREEN LEDS ON
					
		}
		in_loop = 1;
	}
			
	else if ( (temp & END_LOOP) == END_LOOP ){
		//set index to index of the loop
		//if loop counter is = to value, dont change index
		
		if ( !in_loop ){
			//the recipe is bad. there is an END_LOOP before a START_LOOP
			//RECIPE_COMMAND_ERROR
			servos[servo].servo_state = state_nested_error;
			
		}
				
		if ( servos[servo].loop_counter == servos[servo].times_to_loop ){
			//if loop_counter is equal to times_to_loop
			//it has reached the number of times to loop
			//turn off the in loop flag
			in_loop = 0;
		}
				
		else{
		servos[servo].loop_counter++;
		index_number = servos[servo].loop_start_index;
		}
	}
	
	else if ( temp == 0 ){
		//end of recipe
		servos[servo].servo_state = state_recipe_end;
		
	}
			
	else{
		//command unrecognized
		servos[servo].servo_state = state_command_error;
	}

}

void override_process( char input, int servo ){
	if (input == 'P' || input == 'p'){
		if (servos[servo].servo_state != state_recipe_end && 
			servos[servo].servo_state != state_nested_error &&
			servos[servo].servo_state != state_command_error){
				
			servos[servo].servo_state = state_paused;
		}
	}
	
	else if ( input == 'C' || input == 'c' ){
		if (servos[servo].servo_state != state_recipe_end && 
			servos[servo].servo_state != state_nested_error &&
			servos[servo].servo_state != state_command_error){
				
			servos[servo].servo_state = state_running;
		}
	}

	else if ( input == 'R' || input == 'r' ){
		if (servos[servo].servo_state == state_paused &&
			servos[servo].position != 5 ){
			
			Turn_Right( servo );
			Move_Buffering(1);
		}
	}
	
	else if ( input == 'L' || input == 'l' ){
		if (servos[servo].servo_state == state_paused &&
			servos[servo].position != 0 ){
			
			Turn_Left( servo );
			Move_Buffering(1);				
		}		
	}
	
	else if ( input == 'N' || input == 'n' ){
		
	}
	
	else if ( input == 'B' || input == 'b' ){
		servos[servo].current_index = 0;
	}	
}

void Turn_Left( int servo ){
	int pulse_width;
	servos[servo].position--;
	pulse_width = SMALLEST_WIDTH + servos[servo].position*STEP_INTERVAL;
	Change_Width( pulse_width, servo );
}

void Turn_Right( int servo ){
	int pulse_width;
	servos[servo].position++;
	pulse_width = SMALLEST_WIDTH + servos[servo].position*STEP_INTERVAL;
	Change_Width( pulse_width, servo );	
}


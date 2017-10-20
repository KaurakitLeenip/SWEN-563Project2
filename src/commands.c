#include "commands.h"

servo servos[2];
int servo_number;
int override_flag = 0;
status program_status = status_running;
char line[256];

void Move_Buffering( int moves ){
	
	int delay = moves * 1000000;
	USART_Delay(delay);
	
}

void * overrideCheck(){
	int i;
	while(1){
		Read_Line(line);
		for ( i = 0; i < sizeof(line); i++ ){
			if ( line[i] == '>' ){
				override_flag = 1;
			}
		}
	}
}

void Run_State(){
	
	CreateThread( NULL, NULL, overrideCheck, NULL );
	while( servo_state != state_recipe_ended ){
		
		if ( override_flag == 1 ){
			program_status = status_input_read;
		}
		
		 switch( program_status ){
			
			case status_input_read:
				
				Write_Line("\r\n>");
				for ( i = 0; i < sizeof(line); i++ ){
					if (line[i] == 'x' || line[i] == 'X'){
						
					}
					else{
						servo[servo_number].servo_state = state_process_override;
					}
				}
				override_flag = 0;
				

			case status_running:
				
				Red_LED_Off();
				Green_LED_Off();
				Green_LED_On();
			
				for ( servo_number = 0; servo_number < 2; servo_number++ ){
					if ( servo[servo_number].servo_state == state_running ){
						process_recipe( servo[servo_number].current_index, servo_number );
					}
					else if (servo[servo_number].servo_state == state_process_override){
						override_process( line[servo_number], servo_number );
					}
					else if (servos[servo_number].servo_state == state_recipe_end){
						program_status = status_done;
					}
					else{
						//error/pause states do nothing
					}
				}
			case status_done:

		}
	}
}

void process_recipe( int index_number, int servo ){
	
	unsigned char temp;
	temp = recipe[index_number];
	
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
		times_to_loop = temp;
		start_loop_index = i;
		start_loop_index++; 							
		//the next statements will be looped
				
		if( in_loop != 0 ){
		//NESTED_LOOP_ERROR: BOTH RED AND GREEN LEDS ON
					
		}
		in_loop = 1;
	}
			
	else if ( (temp & END_LOOP) == END_LOOP ){
		//set index to index of the loop
		//if loop counter is = to value, dont change index
		
		if ( in_loop ){
			//the recipe is bad. there is an END_LOOP before a START_LOOP
			//RECIPE_COMMAND_ERROR
			servos[servo].servo_state = state_nested_error;
			
		}
				
		if ( loop_counter == times_to_loop ){
			//if loop_counter is equal to times_to_loop
			//it has reached the number of times to loop
			//turn off the in loop flag
			in_loop = 0;
		}
				
		else{
		loop_counter++;
		i = start_loop_index;
		}
	}
	
	else if ( temp == 0 ){
		//end of recipe
		servos[servo]/servo_state = state_recipe_end;
		break;
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
		if (servos[servo.servo_state == state_paused &&
			servos[servo.position != 5 ){
			
			Turn_Right( servo );
			Move_Buffering(1);
		}
	}
	
	else if ( input == 'L' || input == 'l' ){
		if (servos[servo.servo_state == state_paused &&
			servos[servo.position != 0 ){
			
			Turn_Left( servo );
			Move_Buffering(1);				
		}		
	}
	
	else if ( input == 'N' || input == 'n' ){
		
	}
	
	else if ( input == 'B' || input == 'b' ){
		servo[servo].current_index = 0;
	}	
}

void Turn_Left( int servo ){
	servos[servo].position--;
	pulse_width = SMALLEST_WIDTH + servos[servo].position*STEP_INTERVAL;
}

void Turn_Right( int servo ){
	servos[servo].position++;
	pulse_width = SMALLEST_WIDTH + servos[servo].position*STEP_INTERVAL;
}


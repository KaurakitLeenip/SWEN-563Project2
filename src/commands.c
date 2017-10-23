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
int in_loop = 0;
enum status program_status = status_running;
uint8_t line[256];

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

char recipe_loop[100] = {
	MOV | 0,
	LOOP | 3,
	MOV | 4,
	MOV | 5,
	0xA0,
	MOV | 2,
	RECIPE_END
	
};

char recipe_nested[100] = {
	MOV | 0,
	LOOP | 2,
	LOOP | 3,
	END_LOOP,
	END_LOOP,
	RECIPE_END
};

char recipe_command[100] = {
	MOV | 0,
	0xA0,
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
	
	int delay = ( moves + 1 ) * 500000;
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
	Write_Line("\r\n>");
	while( program_status != status_done ){
		int i;
		char input[1];
		
		if (( USART2->ISR & USART_ISR_RXNE )){
			//if the RXNE interrupt has been flagged
			override_flag = 1;


		}
		
		if ( override_flag == 1 ){ 
			program_status = status_input_read;
			rxByte = ((uint8_t)(USART2->RDR & 0xFF));
			input[0] = rxByte;
			line[0] = rxByte;
			Write_Line(input);
		}
		
		 switch( program_status ){
			
			case status_input_read:
				Read_Line(line);
				USART2->RQR |= USART_RQR_RXFRQ;
				if ( line[6] == '>' ){
					for ( i = 0; line[i] != '\0'; i++ ){
						if (line[i] == 'x' || line[i] == 'X'){
							Write_Line("\r\n>");
							servos[0].servo_state = state_running;
							servos[1].servo_state = state_running;
							override_flag = 0;
						}
					}
						if ( override_flag == 1 ){
							servos[0].servo_state = state_process_override;
							servos[1].servo_state = state_process_override;
						}
						
					}
				
				override_flag = 0;
				program_status = status_running;
				

			case status_running:
				
				switch ( servos[0].servo_state ){
				//LED switches	
					case state_running:
						Red_LED_Off();
						Green_LED_Off();
						Green_LED_On();
						break;
					
					case state_paused:
						Red_LED_Off();
						Green_LED_Off();
						break;
					
					case state_nested_error:
						Red_LED_Off();
						Green_LED_Off();
						Red_LED_On();
						Green_LED_On();
						break;
					
					case state_command_error:
						Red_LED_Off();
						Green_LED_Off();
						Red_LED_On();
						break;
				}
				
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
					else if ( USART2->ISR & 0x20 == 0x20 ){
						//if the RXNE interrupt has been flagged
						override_flag = 1;
						line[0] = USART_Read(USART2);
						}
					else{
						//error and pause states do nothing
					}
				}
			case status_done:

		}
	}
}

void process_recipe( int index_number, int servo ){
	
	unsigned char temp;
	int value;
	int opcode;
	
	temp = recipe_loop[index_number];
	opcode = temp >> 5;
	

	
	if ( opcode == 1 ){
				
		int pulse_width;
		int delay_cycles;
		int pos;
				
		temp &= 0x1F;
		value = temp;
		pos = servos[servo].position;
		servos[servo].position = value;
		pos = servos[servo].position;
		pulse_width = SMALLEST_WIDTH + value*STEP_INTERVAL;
		Change_Width( pulse_width, servo );
		delay_cycles = abs(servos[servo].position - value);
		Move_Buffering(delay_cycles);
				
	}
			
	else if ( opcode == 2 ){
		
		int delay;
		temp &= 0x1F;
		value = temp;

		value++;																
		//wait 0 is a wait of 1 cycle so the value must be adjusted
				
		delay = value*100000;
		USART_Delay(delay);
				
		}
			
	else if ( opcode == 4 ){

		//save current index
		temp &= 0x1F;
		servos[servo].times_to_loop = temp;
		servos[servo].loop_start_index = index_number;

		//the next statements will be looped
				
		if( in_loop != 0 ){
		//NESTED_LOOP_ERROR: BOTH RED AND GREEN LEDS ON
					
		}
		in_loop = 1;
	}
			
	else if ( opcode == 5 ){
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
			servos[servo].current_index = servos[servo].loop_start_index;

		}
	}
	
	else if ( opcode == 0 ){
		//end of recipe
		servos[servo].servo_state = state_recipe_end;
		
	}
			
	else{
		//command unrecognized
		servos[servo].servo_state = state_command_error;
	}
}

void override_process( char input, int servo ){
	int p = servos[servo].position;
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
	servos[servo].position++;
	pulse_width = SMALLEST_WIDTH + servos[servo].position*STEP_INTERVAL;
	Change_Width( pulse_width, servo );
}

void Turn_Right( int servo ){
	int pulse_width;
	servos[servo].position--;
	pulse_width = SMALLEST_WIDTH + servos[servo].position*STEP_INTERVAL;
	Change_Width( pulse_width, servo );	
}


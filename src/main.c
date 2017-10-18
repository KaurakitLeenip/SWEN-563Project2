#include "stm32l476xx.h"
#include "SysClock.h"
#include "UART.h"
#include "Util.h"
#include "Timer.h"
#include "LED.h"
#include "Commands.h"

#include <string.h>
#include <stdio.h>

#define ENTER_ASCII (13)
#define MOV (0x20)
#define WAIT (0x40)
#define LOOP (0x80)
#define END_LOOP (0xA0)
#define RECIPE_END (0)
#define STEP_INTERVAL (320)
#define SMALLEST_WIDTH (400)
#define DEFAULT_STATE (3)

char RxComByte = 0;
uint8_t buffer[BufferSize];
unsigned char recipe1[] = { MOV|3, MOV|5, RECIPE_END};

char * strRead = "";

int main(void){
	
	int i;
	int start_loop_index;
	int times_to_loop;
	int loop_counter = 0;
	int width = 400;
	int value;
	unsigned char temp;
	int in_loop;
	
	
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   ALWAYS MAKE SURE TO INITIALIZE SysClock and UART FIRST !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	System_Clock_Init();
	UART2_Init();
	
	LED_Init();
	Left_Timer_Init();
	Right_Timer_Init();
	Start_Timer_Left();
	Start_Timer_Right();
	Move_Buffering(5);
	Change_Width_Left( SMALLEST_WIDTH + DEFAULT_STATE * STEP_INTERVAL );
	Move_Buffering(5);


		for ( i = 0; i < sizeof(recipe1); i++ ){
			
			temp = recipe1[i];
			
			if ( (temp & MOV) == MOV ){
				
				int pulse_width;
				
				temp &= 0x1F;
				value = temp;
				pulse_width = SMALLEST_WIDTH + value*STEP_INTERVAL;
				
				Change_Width_Left( pulse_width );
				
			}
			
			else if ( (temp & WAIT) == WAIT ){
				int delay;
				temp &= 0x1F;
				value = temp;

				value++;																
				//wait 0 is a wait of 1 cycle so the value must be adjusted
				
				delay = value*100000;
				USART_Delay(delay);
				//THIS MIGHT BE THE WRONG IMPLEMENTATION
				
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
			
			else{
			//it is the recipe end
				break;
			}
		}
	
	
	
	
}



#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "POST.h"
#include "Util.h"
#include "Histogram.h"

#include <string.h>
#include <stdio.h>

#define MAX_BUCKETS (101)
#define ENTER_ASCII (13)

char RxComByte = 0;
char string_to_echo[]="";
uint8_t buffer[BufferSize];
unsigned short lower_limit = 950;
unsigned short upper_limit;

char * strRead = "";

void Collect_Data( int raw_data[] ){
	
	short max_measurements = 1001;
	short current_time;
	short current_number_of_measurements;
	int index = 0;
	int startTime;
	
	while( current_number_of_measurements < max_measurements ){
		
		Start_Timer();	
		startTime = Get_Timer();
		
		while(1)
		{
			if ( Event_Captured() ){
				current_time = Get_Timer() - startTime;
				Stop_Timer();

				if ( current_time >= lower_limit && current_time < upper_limit ){
					//record time
					index = current_time - lower_limit;
					raw_data[index]++;
				}
				
				break;
			}
		}
		current_number_of_measurements++;
		index++;
	}
	
	Stop_Timer();
}

int main(void){

	int POST_result;
	int raw_data[MAX_BUCKETS];
	int print_result;
	int i;
	
	char rxByte;
	char print_success[] = "Logging Complete. Would you like to run again? (y/n) \r\n";
	char exit_statement[] = "Exiting...\r\n";
	
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   ALWAYS MAKE SURE TO INITIALIZE SysClock and UART FIRST !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	System_Clock_Init();
	UART2_Init();
	
	Init_PA0();
	POST_result = Run_POST();
	
	if ( POST_result == 0 ){
		POST_result = POST_FAILED();
	}
	
	if ( POST_result == 1 ){
		Write_Line( "POST Successful. \r\n" );
		while(1){
			lower_limit = POST_PASSED();
			upper_limit = lower_limit + 100;
			//collect data
			//initializing all values to zero
			for( i = 0; i < sizeof(raw_data); i++ ){
				raw_data[i] = 0;
			}
			Collect_Data( raw_data );
			print_result = Print_Results( raw_data, lower_limit );
			
			Write_Line( print_success );
			rxByte = USART_Read(USART2);
			Write_Line("\r\n");
			if( rxByte == 'y' || rxByte == 'Y' ){
				lower_limit = 950;
			}
			else if( rxByte == 'n' || rxByte == 'N' ){
				Write_Line( exit_statement );
				return 0;				
			}
		}
		
		return 0;
	}	
}



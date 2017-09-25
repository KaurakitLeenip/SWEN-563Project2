/*
	FILE DESCRIPTHION HERE

*/

#include "POST.h"
#include "Timer.h"
#include "UART.h"
#include "Util.h"

#define MAX_BUCKETS (101)
#define ENTER_ASCII (13)

/* 
 *Initializes GPIO port PA0 so we can use it as an input.  Also enables the alternate function mode and then initializes the timer
*/
void Init_PA0( void ){
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN ;       // enable GPIOA clock
    GPIOA->MODER &= ~0xF ;                      // put PA0 into input mode by clearing bits to 0x00
    GPIOA->MODER |= 0x02 ;                      // Enable alternate function mode (binary 10) for PA0
    Timer_Init();                               // initialize the timer
}

/*
 * runs post
*/
int Run_POST()
{
	unsigned int max_time = 100000;
	unsigned int current_time;
	//Stop_Timer();
	//Start_Timer();
	
	int us = 100000;
	int delay = 100*us/7;
	
	while(delay--){

		if ( Event_Captured() ){
			//POST SUCCESSFUL
			return 1;
		}
	}
	return 0;
}

int POST_FAILED()
{
	
	char fail_message[] = "POST failed.  Would you like to try again? (y/n)\r\n";
	char bad_input[] = "Unrecognized command.  Please only enter (y) or (n)\r\n";
	char rxByte;
	int passed = 0;
	
	Write_Line( fail_message );
	while(1)
	{
		rxByte = USART_Read(USART2);
		if(rxByte == 'y' || rxByte == 'Y')
		{
			Write_Line("\r\n");
			passed = Run_POST();
			if(passed != 0)
			{
				//POST_PASSED();
				return 1;
			}
			else if(passed == 0)
			{
				POST_FAILED();
			}
		}
		else if(rxByte == 'n' || rxByte == 'N')
		{
			return 0;				
		}
		else{			
			USART_Write(USART2, (uint8_t*)bad_input, strlen(bad_input));
			USART_Write(USART2, (uint8_t*)fail_message, strlen(fail_message));
			return 0;
		}
	}
}

int POST_PASSED()
{
	int lower_limit = 950;
	int upper_limit = 0;

	char lower_lim_msg[] = "Please input the lower limit: (50-9950) \r\n";
	char upper_lim_msg[] = "Upper limit is: ";
	char lower_lim_report_msg[] = "Lower limit is: ";
	char default_msg[] = "Default values set to 950 and 1050.  Would you like to change? (y/n)\r\n";
	char ready_to_start[] = "Please press enter to continue:\r\n";
	char rxByte;
	char str[255];
	char *line;
	
	USART_Write(USART2, (uint8_t*)default_msg, strlen(default_msg));
	rxByte = USART_Read(USART2);
	Write_Line("\r\n");
	
	if(rxByte == 'y' || rxByte == 'Y'){
		USART_Write(USART2, (uint8_t*)lower_lim_msg, strlen(lower_lim_msg));
		line = Read_Line(str);
		Remove_Spaces(line);
		lower_limit = Str_To_Int(line);		
	}
	
	else if(rxByte == 'n' || rxByte == 'N'){
		//use default values 950 - 1050
	}
	
	// set our upper limit; it'll always be 100 higher than the lower limit,
	// then print it
	upper_limit = lower_limit + 100;
	Write_Line(upper_lim_msg);
	// USART_Write(USART2, (uint8_t *)upper_lim_msg, strlen(upper_lim_msg));
	
	// sprintf essentially formats the input string, in this case str, and sets it to whatever is in the
	// second parameter; in this case, %d, which is a double value.  The last value is the double value which
	// we're including / adding into the string.  Note that means you could have something like 
	// "blahblah: %d", 69
	sprintf(str, "%d", upper_limit);
	Write_Line(str);
	Write_Line("\r\n");
	
	Write_Line(lower_lim_report_msg);
	sprintf(str, "%d", lower_limit);
	Write_Line(str);	
	Write_Line("\r\n");
	Write_Line(ready_to_start);
	
	while(rxByte != ENTER_ASCII){
		rxByte = USART_Read(USART2);
		Write_Line("\r\n");		
	}	
	return lower_limit;
}

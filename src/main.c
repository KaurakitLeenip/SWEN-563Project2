#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "Util.h"
#include "Histogram.h"

#include <string.h>
#include <stdio.h>

#define MAX_BUCKETS (101)
#define ENTER_ASCII (13)

char RxComByte = 0;
uint8_t buffer[BufferSize];

char * strRead = "";

int main(void){

	int POST_result;
	
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   ALWAYS MAKE SURE TO INITIALIZE SysClock and UART FIRST !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	System_Clock_Init();
	UART2_Init();S
	
	Init_PA0();
	POST_result = Run_POST();
	
	if ( POST_result == 0 ){
		POST_result = POST_FAILED();
	}
	
	if ( POST_result == 1 ){
		Write_Line( "POST Successful. \r\n" );
		while(1){
			
		}
		
		return 0;
	}	
}



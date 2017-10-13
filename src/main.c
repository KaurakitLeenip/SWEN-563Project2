#include "stm32l476xx.h"
#include "SysClock.h"
#include "UART.h"
#include "Util.h"
#include "Timer.h"
#include "LED.h"

#include <string.h>
#include <stdio.h>

#define ENTER_ASCII (13)

char RxComByte = 0;
uint8_t buffer[BufferSize];

char * strRead = "";

int main(void){
	
	int us = 100000;
	int delay = 100*us/7;
	
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   ALWAYS MAKE SURE TO INITIALIZE SysClock and UART FIRST !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	System_Clock_Init();
	UART2_Init();
	
	LED_Init();
	Timer_Init();
	//Change_Width(20);
	//while(1){
	Start_Timer();
	Change_Width(4);
	//}
	/*while(1){
		
		while(delay--){
		
			
			
		}
	
	
		delay = 100*us/7;									//reset interrupt timer
	}
	*/
}



#include "stm32l476xx.h"
#include "SysClock.h"
#include "UART.h"
#include "Util.h"
#include "Timer.h"
#include "LED.h"
#include "commands.h"

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

int main(void){
	
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   ALWAYS MAKE SURE TO INITIALIZE SysClock and UART FIRST !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	System_Clock_Init();
	UART2_Init();
	
	LED_Init();
	Timer_Init();
	Start_Timer();
	Init_Servos();
	//Change_Width(2000,1);
	//Change_Width(2000,0);
	Move_Buffering(5);
	Run_State();


	


}

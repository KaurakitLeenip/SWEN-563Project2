#include "commands.h"
#include "Timer.h"
#include "UART.h"

void Move_Buffering( int moves ){
	
	int delay = moves * 1000000;
	USART_Delay(delay);
	
}

void Move_Servo_Pos( int position ){
	
	switch( position ){
		
		case 0:
			//Change_Width(0);
		
		case 1:
			//Change_Width(0);
		
		case 2:
			//Change_Width(0);
		
		case 3:
			//Change_Width(0);
		
		case 4:
			//Change_Width(0);
		
		case 5:
			//Change_Width(0);		
	}
	
}

void Wait( int cycles ){
	
	int us = cycles*100000;
	int delay = 100*us/7;
	
	while(delay--){
		
			
			
	}
}

void Loop( int loops ){
	
}

void Turn_Left(){
	
}

void Turn_Right(){
	
}

void Pause(){
	
}

void Continue(){
	
}
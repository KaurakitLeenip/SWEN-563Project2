#ifndef TIMER_H
#define TIMER_H

#include "stm32l476xx.h"
#include "SysClock.h"
#include "Timer.h"    

#include <string.h>
#include <stdio.h>

#define SYSTEM_CLK 80   // STM 80MHZ

void Left_Timer_Init( void ) ;

void Right_Timer_Init( void ) ;

void Start_Timer_Left( void ) ;

void Start_Timer_Right( void ) ;

void Stop_Timer_Left( void ) ;

void Stop_Timer_Right( void ) ;

void Change_Width_Left( int width ) ;

void Change_Width_Right( int width ) ; 


#endif /* TIMER_H */

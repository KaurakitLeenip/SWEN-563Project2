#ifndef TIMER_H
#define TIMER_H

#include "stm32l476xx.h"
#include "SysClock.h"
#include "Timer.h"    

#include <string.h>
#include <stdio.h>

#define SYSTEM_CLK 80   // STM 80MHZ

void Timer_Init( void ) ;

void Start_Timer( void ) ;

void Stop_Timer( void ) ;

void Change_Width( int width, int servo ) ;


#endif /* TIMER_H */

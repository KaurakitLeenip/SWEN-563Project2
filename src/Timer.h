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

unsigned int Get_Timer( void ) ;

void Stop_Timer( void ) ;

int Event_Captured( void ) ;

#endif /* TIMER_H */

#ifndef POST_H
#define POST_H

#include "stm32l476xx.h"
#include "Timer.h"
#include "UART.h"
#include "Util.h"
#include <String.h>

void Init_PA0( void ) ;

int Run_POST( void );

int POST_FAILED( void );

int POST_PASSED( void );

#endif /* POST_H */

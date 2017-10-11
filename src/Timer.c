#include "Timer.h"

/* 
 *Configure the timer on PA0.  Need to enable alternate function, enable the timer clock and adjust the prescale.
 * From there, need to create the events for the prescale
*/
void Timer_Init( void ) {
	
    GPIOA->AFR[0] |= 0x1;                   // set alternate function for PA0; using AF1 (TIM2_CH1)
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;   // TIM2 timer clock enable
    TIM2->PSC = SYSTEM_CLK;                 // prescale the system clock
    TIM2->EGR |= TIM_EGR_UG;								// set the update generation bit; see RM for more info, but forces an update of the register

    TIM2->CCMR1 |= 0x68;                    // setup channel for input and output compare 1 preload, enable PWN mode 1
		TIM2->CR1 |= 0x80;											// Enable auto-reload preload enable
		TIM2->CCER |= 0x01;											// enable channel 1 output bit
		TIM2->ARR |= 0x14;
	
    TIM2->EGR |= TIM_EGR_UG;								// set the update generation bit; see RM for more info, but forces an update of the register

}

/* Start timer by enabling the control register bit */
void Start_Timer( void ) {
    TIM2->CR1 = 0x1;                        // control register enabled to start timing
}

/* This stops the timer by disabling the control register. */
void Stop_Timer( void ) {
    TIM2->CR1 = 0x0;                        // control register diabled
}

/* Change the pulse width of the PWM */
void Change_Pulse( int width ){
		TIM2 -> CCR1 = width;
    TIM2->EGR |= TIM_EGR_UG;			
}




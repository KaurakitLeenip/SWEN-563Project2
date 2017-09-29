#include "Timer.h"

/* 
 *Configure the timer on PA0.  Need to enable alternate function, enable the timer clock and adjust the prescale.
 * From there, need to create the events for the prescale
*/
void Timer_Init( void ) {
    GPIOA->AFR[0] |= 0x8;                   // set alternate function for PA8; using AF1 (TIM1_CH1)
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;     // TIM1 timer clock enable
    TIM2->PSC = SYSTEM_CLK;                 // prescale the system clock
    TIM2->EGR |= TIM_EGR_UG;                // set the update generation bit; see RM for more info, but forces an update of the register
    TIM2->CCER &= ~(0xFFFFFFFF);            // hold off on getting inputs until we're ready for them
    TIM2->CCMR1 |= 0x1;                     // setup channel for input and clear the filter
    TIM2->CCER |= 0x1;                      // start capturing input

}

/* Start timer by enabling the control register bit */
void Start_Timer( void ) {
    TIM2->CR1 = 0x1;                        // control register enabled to start timing
}

/* 
 * Gets the value in the capture register.  This is the time elapsed since the timer was enabled, in us
*/
unsigned int Get_Timer( void ) {
    return (unsigned int)TIM2->CCR1;        // get capture register value
}

/* This stops the timer by disabling the control register. */
void Stop_Timer( void ) {
    TIM2->CR1 = 0x0;                        // control register diabled
}

/* 
 * Determines if an edge (aka an event) has been seen. The return value is the 
 * flag of the status registers first bit. 
*/
int Event_Captured( void ) {
    return (TIM2->SR & 0x2);                // return flag indicating capture interrupt flag
}



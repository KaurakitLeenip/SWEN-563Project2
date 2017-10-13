#include "Timer.h"
#include "UART.h"

/* 
 *Configure the timer on PA0.  Need to enable alternate function, enable the timer clock and adjust the prescale.
 * From there, need to create the events for the prescale
*/
void Timer_Init( void ) {

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN ;       // enable GPIOA clock
    GPIOA->MODER &= ~0xF ;  					// clear GPIOA bits
	GPIOA->MODER |= 0x02 ;                      // Enable alternate function mode (binary 10) for PA0
	
    GPIOA->AFR[0] |= 0x1;                   	// set alternate function for PA0; using AF1 (TIM2_CH1)
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;   	// TIM2 timer clock enable

    TIM2->PSC = SYSTEM_CLK;				        // set prescale to 80 us per count
    TIM2->EGR |= TIM_EGR_UG;					// set the update generation bit; see RM for more info, but forces an update of the register

    TIM2->CCMR1 |= 0x68;                    	// setup channel for output compare 1 preload, enable PWN mode 1
	TIM2->CCMR1 &= 0xFC;
	TIM2->CR1 |= 0x80;							// Enable auto-reload preload enable
	TIM2->CCER |= 0x01;							// enable channel 1 output bit

	TIM2->ARR = 0x4E20;							// set the period of the PWN to 200 
	TIM2->CCR1 = 0x7D0;							// set the width of the pwn to 20
	
    TIM2->EGR |= TIM_EGR_UG;					// set the update generation bit; see RM for more info, but forces an update of the register

		
		
}

/* Start timer by enabling the control register bit */
void Start_Timer( void ) {
    TIM2->CR1 |= 0x1;							// control register enabled to start timing
}

/* This stops the timer by disabling the control register. */
void Stop_Timer( void ) {
    TIM2->CR1 &= 0xFFFE;                        // control register diabled
}

/* Change the pulse width of the PWM */
void Change_Width( int width ){
	int real_width = width * 100; 				// changing us to ms
	TIM2 -> CCR1 = real_width;
    TIM2->EGR |= TIM_EGR_UG;			
}




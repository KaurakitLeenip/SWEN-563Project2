#include "Timer.h"
#include "UART.h"

/* 
 * SERVO #5, its got a 17 on it and used to be bad
 * 750 width to hit 0 marker
 *
 * 
*/
void Left_Timer_Init( void ) {

  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN ;       // enable GPIOA clock
  GPIOA->MODER &= ~0xF ;  										// clear GPIOA bits
	GPIOA->MODER |= 0x02 ;                      // Enable alternate function mode (binary 10) for PA0
	
  GPIOA->AFR[0] |= 0x1;                   		// set alternate function for PA0; using AF1 (TIM2_CH1)
  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;   		// TIM2 timer clock enable

  TIM2->PSC = 100;				        			// set prescale to 80 us per count
  TIM2->EGR |= TIM_EGR_UG;										// set the update generation bit; see RM for more info, but forces an update of the register

  TIM2->CCMR1 |= 0x68;                    		// setup channel for output compare 1 preload, enable PWN mode 1
	TIM2->CCMR1 &= 0xFC;												// set CCMR to be output
	TIM2->CR1 |= 0x80;													// Enable auto-reload preload enable
	TIM2->CCER |= 0x01;													// enable channel 1 output bit

	TIM2->ARR = 0x4E20;													// set the period of the PWN to 2000 
	TIM2->CCR1 = 400;														// set the width of the pwn to 400
	
  TIM2->EGR |= TIM_EGR_UG;										// set the update generation bit; see RM for more info, but forces an update of the register
		
}

void Right_Timer_Init( void ) {

	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN ;       // enable GPIOA clock
  GPIOB->MODER &= ~0xF ;  										// clear GPIOA bits
	GPIOB->MODER |= 0x2000 ;         						// Enable alternate function mode (binary 10) for PA0

	GPIOB->AFR[0] |= 0x2000000;
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
	
	TIM4->PSC = SYSTEM_CLK;
	TIM4->EGR |= TIM_EGR_UG;
	
	TIM4->CCMR1 |= 0x68;                    		// setup channel for output compare 1 preload, enable PWN mode 1
	TIM4->CCMR1 &= 0xFC;												// set CCMR to be output
	TIM4->CR1 |= 0x80;													// Enable auto-reload preload enable
	TIM4->CCER |= 0x01;													// enable channel 1 output bit

	TIM4->ARR = 0x4E20;													// set the period of the PWN to 200 
	TIM4->CCR1 = 0x7D0;													// set the width of the pwn to 20
	
  TIM4->EGR |= TIM_EGR_UG;										// set the update generation bit; see RM for more info, but forces an update of the register

	
}

/* Start timer by enabling the control register bit */
void Start_Timer_Left( void ) {
    TIM2->CR1 |= 0x01;													// control register enabled to start timing
}

void Start_Timer_Right( void ){
		TIM4->CR1 |= 0x0l;
}

/* This stops the timer by disabling the control register. */
void Stop_Timer_Left( void ) {
    TIM2->CR1 &= 0xFFFE;                       // control register diabled
}


void Stop_Timer_Right( void ){
		TIM4->CR1 &= 0xFFFE;
}

/* Change the pulse width of the PWM */
void Change_Width_Left( int width ){

	TIM2->CCR1 = width;
  TIM2->EGR |= TIM_EGR_UG;			

}

void Change_Width_Right( int width ){

	TIM5->CCR1 = width;
	TIM5->EGR |= TIM_EGR_UG;
	
}


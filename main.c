#include "stm32f4xx.h"

#define GPIOAEN (1U<<0)
#define PIN5    (1U<<5)
#define LED_PIN  PIN5

int main(){
	/*ENABLE ACCESS TO GPIOA*/
	RCC->AHB1ENR|=GPIOAEN;
	/*SET PA5TO OP MODE*/
	GPIOA->MODER|=(1U<<10);
	GPIOA->MODER&=~(1U<<11);
	while(1){
		GPIOA->ODR^=LED_PIN;
		for(int i=0;i<100000;i++){

		}

	}
}

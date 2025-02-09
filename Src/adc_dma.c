#include "adc_dma.h"
#define GPIOAEN (1U<<0)
#define ADC1EN (1U<<8)
#define CR1_SCAN (1U<<8)
#define CR2_DMA (1U<<8)
#define CR2_DDS (1U<<9)
#define CR2_CONT (1U<<1)
#define CR2_ADCON (1U<<0)
#define CR2_SWSTART (1U<<30)

#define DMA2EN (1U<<22)
#define DMA_SCR_EN (1U<<0)
#define DMA_SCR_MINC (1U<<10)
#define DMA_SCR_PINC (1U<<9)
#define DMA_SCR_CIRC (1U<<8)
#define DMA_SCR_TCIE (1U<<4)
#define DMA_SCR_TEIE (1U<<2)
#define DMA_SFCR_DMDIS (1U<<2)

#define TIM2EN (1U<<0)
#define CR1_CEN  (1U<<0)

uint16_t adc_raw_data[NUM_OF_SAMPLES];

void adc_tim_dma_init(void){
	/*gpio configuration*/
	/*enable clk access to gpio pins*/
	RCC->AHB1ENR|=GPIOAEN;
	/*set pa0  mode to analog mode*/
	GPIOA->MODER|=(1U<<0);
	GPIOA->MODER|=(1U<<1);


	/*adc config*/
	/*enable clk access to adc*/
	RCC->APB2ENR|=ADC1EN;

	/*select to use DMA*/
	ADC1->CR2|=CR2_DMA|CR2_DDS;

	/*SELECT EXTERNAL TRIGGER ON RISING EDGE*/
	ADC1->CR2|=(1U<<28);
	ADC1->CR2&=~(1U<<29);

	/*SELECT TIMER 2 TRGO EVENT*/
	ADC1->CR2&=~(1U<<24);
	ADC1->CR2|=(1U<<25);
	ADC1->CR2|=(1U<<26);
	ADC1->CR2&=~(1U<<27);

	/*dma config*/
	/*enable clk access to dma*/
	RCC->AHB1ENR|=DMA2EN;
	/*disable dma stream*/
	DMA2_Stream0->CR&=~DMA_SCR_EN;
	/*wait till dma is disabled*/
	while((DMA2_Stream0->CR & DMA_SCR_EN)){

	}
	/*enable circular mode*/
	DMA2_Stream0->CR|=DMA_SCR_CIRC;
	/*enable mem transfer time*/
	DMA2_Stream0->CR|=(1U<<13);
	DMA2_Stream0->CR&=~(1U<<14);

	/*set per transfer size*/
	DMA2_Stream0->CR|=(1U<<11);
	DMA2_Stream0->CR&=~(1U<<12);

	/*en mem increment*/
	DMA2_Stream0->CR|=DMA_SCR_MINC;

	/*EN DMA TRANSFER COMPLETE*/
	DMA2_Stream0->CR|=DMA_SCR_TCIE;

	/*set periph address*/
	DMA2_Stream0->PAR=(uint32_t)(&(ADC1->DR));
	/*set mem address*/
	DMA2_Stream0->M0AR=(uint32_t)(&adc_raw_data);
	/*set no of transfer*/
	DMA2_Stream0->NDTR=(uint16_t)NUM_OF_SAMPLES;

	/*ENABLE DMA INTERRUPT IN NVIC*/
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);

	/*CONFIG TIMER PERIOD TO 100 HZ IE ADC IS SAMPLING AT A 100 HZ EVERY 10ms*/
	/*ENABLE CLK ACCES TO TIM2*/
	RCC->APB1ENR|=TIM2EN;

	/*SET TIM PRESCALAR VALUE*/
	TIM2->PSC=16000-1; //16000000/16000=1000
	/*SET TIME AUTO RELOAD VALUE*/
	TIM2->ARR=10-1; //1000/10=100HZ
	/*CONFIGURATION MASTER MODE SELECTION BITS*/
	TIM2->CR2&=~(1U<<4);
	TIM2->CR2|=(1U<<5);
	TIM2->CR2&=~(1U<<6);
	/*ENABLE MODULES*/
		/*enable adc*/
		ADC1->CR2|=CR2_ADCON;

		/*enable dma stream*/
		DMA2_Stream0->CR|=DMA_SCR_EN;
		/*ENABLE TIMER*/
		TIM2->CR1|=CR1_CEN;

}
void adc_dma_init(void){
	/*gpio configuration*/
	/*enable clk access to gpio pins*/
	RCC->AHB1ENR|=GPIOAEN;
	/*set pa0 and pa1 mode to analog mode*/
	GPIOA->MODER|=(1U<<0);
	GPIOA->MODER|=(1U<<1);
	GPIOA->MODER|=(1U<<2);
	GPIOA->MODER|=(1U<<3);

	/*adc config*/
	/*enable clk access to adc*/
	RCC->APB2ENR|=ADC1EN;
	/*set sequence length*/
	ADC1->SQR1|=(1U<<20);
	ADC1->SQR1=~(1U<<21);
	ADC1->SQR1=~(1U<<22);
	ADC1->SQR1=~(1U<<23);



	/*set sequence*/
	ADC1->SQR3=(0U<<0)|(1U<<5);
	/*enable scan mode*/
	ADC1->CR1=CR1_SCAN;
	/*select to use DMA*/
	ADC1->CR2|=CR2_CONT|CR2_DMA|CR2_DDS;

	/*dma config*/
	/*enable clk access to dma*/
	RCC->AHB1ENR|=DMA2EN;
	/*disable dma stream*/
	DMA2_Stream0->CR&=~DMA_SCR_EN;
	/*wait till dma is disabled*/
	while((DMA2_Stream0->CR & DMA_SCR_EN)){

	}
	/*enable circular mode*/
	DMA2_Stream0->CR|=DMA_SCR_CIRC;
	/*enable mem transfer time*/
	DMA2_Stream0->CR|=(1U<<13);
	DMA2_Stream0->CR&=~(1U<<14);

	/*set per transfer size*/
	DMA2_Stream0->CR|=(1U<<11);
	DMA2_Stream0->CR&=~(1U<<12);

	/*en mem increment*/
	DMA2_Stream0->CR|=DMA_SCR_MINC;

	/*set periph address*/
	DMA2_Stream0->PAR=(uint32_t)(&(ADC1->DR));
	/*set mem address*/
	DMA2_Stream0->M0AR=(uint32_t)(&adc_raw_data);
	/*set no of transfer*/
	DMA2_Stream0->NDTR=(uint16_t)NUM_OF_CHANNELS;
	/*enable dma stream*/
    DMA2_Stream0->CR|=DMA_SCR_EN;
	/*enable adc*/
	ADC1->CR2|=CR2_ADCON;
	/*start adc*/
	ADC1->CR2|=CR2_SWSTART;
}

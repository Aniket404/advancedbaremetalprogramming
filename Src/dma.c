#include "dma.h"

#define DMA2EN (1U<<22)
#define DMA_SCR_EN (1U<<0)
#define DMA_SCR_MINC (1U<<10)
#define DMA_SCR_PINC (1U<<9)
#define DMA_SCR_TCIE (1U<<4)
#define DMA_SCR_TEIE (1U<<2)
#define DMA_SFCR_DMDIS (1U<<2)

void dma2_mem2mem_config(void){
	/*En clk access to dma module*/
	RCC->AHB1ENR|=DMA2EN;
	/*disable dma stream*/
	DMA2_Stream0->CR=0;
	/*wait util stream is disabled*/
	while((DMA2_Stream0->CR& DMA_SCR_EN)){

	}
	/*config dma parameters*/
	/*SET MSIZE MEMORY DATA SIZE TO HALF WORD*/
	DMA2_Stream0->CR|=(1U<<13);
	DMA2_Stream0->CR&=~(1U<<14);
	/*SET PERIPHERAL DATA SIZE DATA SIZE TO HALF WORD*/
	DMA2_Stream0->CR|=(1U<<11);
	DMA2_Stream0->CR&=~(1U<<12);

	/*ENABLE MEMORY INCREMENT*/
	DMA2_Stream0->CR|=DMA_SCR_MINC;
	/*ENABLE PERIPHERAL INCREMENT*/
	DMA2_Stream0->CR|=DMA_SCR_PINC;
	/*SELECT MEM TO MEM TRANSFER*/
	DMA2_Stream0->CR|=(1U<<7);
	DMA2_Stream0->CR&=~(1U<<6);

	/*ENABLE TRANSFER COMPLETE INTERRUPT*/
	DMA2_Stream0->CR|=DMA_SCR_TCIE;
	/*ENABLE TRANSFER ERROR INTERRUPT*/
	DMA2_Stream0->CR|=DMA_SCR_TEIE;

	/*disable direct mode*/
	DMA2_Stream0->FCR|=DMA_SFCR_DMDIS;

	/* set dma fifo threshold*/
	DMA2_Stream0->FCR|=(1U<<0);
	DMA2_Stream0->FCR|=(1U<<1);

	/*enable dma interrupt in nvic*/
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

void dma_transfer_start(uint32_t src_buff,uint32_t dest_buff,uint32_t len){
	/*set peripheral address*/
	DMA2_Stream0 ->PAR=src_buff;
	/*set memory address*/
	DMA2_Stream0->M0AR=dest_buff;

	/*set transfer length*/
	DMA2_Stream0-> NDTR=len;
	/*enable dma stream*/
	DMA2_Stream0->CR|=DMA_SCR_EN;


}

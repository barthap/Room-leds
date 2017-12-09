/*
 * DMA.c
 *
 * Created: 2013-01-26 18:39:22
 *  Author: tmf
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "DMA.h"

void DMA_init()
{
	DMA.CH0.DESTADDR0=(uint8_t)(uint16_t)&USARTD0_DATA;       //Adres rejestru danych USART
	DMA.CH0.DESTADDR1=((uint16_t)&USARTD0_DATA)>>8;
	DMA.CH0.DESTADDR2=0;
	
	DMA.CTRL=DMA_ENABLE_bm;                //Odblokuj kontroler, round robin
	
	DMA.CH0.CTRLB=DMA_CH_TRNINTLVL_LO_gc;  //Odblokuj przerwanie koñca transakcji DMA
	PMIC_CTRL|=PMIC_LOLVLEN_bm;            //Odblokuj poziom przerwañ w kontrolerze PMIC
	DMA.CH0.ADDRCTRL=DMA_CH_SRCDIR_INC_gc; //Adres Ÿród³owy jest inkrementowany przy transferze, docelowy nie
	DMA.CH0.TRIGSRC=DMA_CH_TRIGSRC_USARTD0_DRE_gc; //Wyzwalanie kana³u - wolne miejsce w buforze nadajnika
}

void DMA_InitTransfer(void *src, uint16_t len, DMA_CH_t *channel)
{
	channel->SRCADDR0=(uint16_t)src;
	channel->SRCADDR1=((uint16_t)src)>>8;
	channel->SRCADDR2=0;                    //Nie korzystamy z najstarszych 8 bitów adresu
	channel->TRFCNT=len;
	channel->CTRLA=DMA_CH_ENABLE_bm | DMA_CH_SINGLE_bm; //Zainicjuj transfer DMA - tryb single shot, 1 bajt/transfer
}

/*
 * DMA.h
 *
 * Created: 2013-01-26 18:38:10
 *  Author: tmf
 */ 


#ifndef DMA_H_
#define DMA_H_

#include <stdint.h>

void DMA_init();              //Zainicjuj kana³ CH0 DMA dla nadajnika USARTD0
void DMA_InitTransfer(void *src, uint16_t len, DMA_CH_t *channel); //Zainicjuj jeden transfer DMA z podanego adresu, len bajtów

#endif /* DMA_H_ */
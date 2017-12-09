/*
 * bufusart.c
 *
 * Created: 2013-01-24 18:13:44
 *  Author: tmf
 */ 

#include "bufusart.h"
#include "usart.h"
#include "DMA.h"
#include "malloc.h"
#include <avr/pgmspace.h>
#include <util/atomic.h>
#include <stdlib.h>
#include <string.h>

CircBuffer recBuf;
CircSendBuffer sendBuf;
volatile uint8_t cmdrec=0;

volatile char *DMA_buf;   //Wska�nik na bufor danych do transmisji 

bool cb_Send_Add(CircSendBuffer *cb, char *elem)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)       //Trwa nadawanie, wi�c tylko dodajemy do kolejki
	{

		if((DMA_CH0_CTRLB & (DMA_CH_CHBUSY_bm | DMA_CH_TRNIF_bm))==0)
		{  //Musimy zainicjowa� transfer
			DMA_buf=elem;
			DMA_InitTransfer(DMA_buf, strlen(DMA_buf), &DMA.CH0);
			return true;
		}

		if(cb_Send_IsFull(cb)) return false;         //Czy jest miejsce w kolejce?
		uint8_t end = (cb->Beg + cb->Count) % CB_SEND_MAXTRANS;
		cb->elements[end] = elem;              //Dodaj transakcj�
		++cb->Count;                           //Liczba element�w w buforze
	}	
	return true;      //Wszystko ok
}

char *cb_Send_Read(CircSendBuffer *cb)
{
	char *elem;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		if(cb_Send_IsEmpty(cb)) return 0;       //Bufor pusty, nie mo�na zwr�ci� elementu
		elem = cb->elements[cb->Beg];
		cb->Beg = (cb->Beg + 1) % CB_SEND_MAXTRANS;
		-- cb->Count;                        //Zmniejszamy liczb� element�w pozosta�ych
	}		                                 //w buforze
	return elem;
}

ISR(USARTD0_RXC_vect)
{
	uint8_t ch=USARTD0_DATA;
	if(ch=='\r' || ch=='\n')
	{
		ch=0;
		++cmdrec;
	}
	cb_Add(&recBuf, ch);
}
ISR(DMA_CH0_vect)        //Koniec poprzedniego transferu, sprawd�my czy czeka kolejny
{
	DMA_CH0_CTRLB|=DMA_CH_TRNIF_bm;  //Kasujemy flag� - ze wzgl�du na wsp�dzielenie wektora nie jest kasowana automatycznie
	free(DMA_buf);
	if(cb_Send_IsEmpty(&sendBuf)==false)
	{                   //Czekaj� dane
		DMA_buf=cb_Send_Read(&sendBuf);
		DMA_InitTransfer(DMA_buf, strlen(DMA_buf), &DMA.CH0);
	}
}
void USART_send_buf_F(CircSendBuffer *buf, const char *txt)
{
	char *buftxt=(char*)malloc_re(strlen_P(txt)+1);  //Przydziel pami�� na tekst
	strcpy_P(buftxt, txt);                 //Skopiuj �a�cuch tekstowy
	cb_Send_Add(&sendBuf, buftxt);         //TODO::TYLKOBLUETOOTH
}

/* 
* Interface.cpp
*
* Created: 2014-04-22 16:23:26
* Author: Barthap
*/
#include <util/atomic.h>

extern "C"
{
	#include "usart.h"
	#include "bufusart.h"
	#include "RingBuffer.h"
	#include "DMA.h"
};


#include "Interface.h"

namespace USART
{
	
	uint8_t parse_buffer[PARSE_BUFFER_SIZE];
	
void Init()
{
	PORTD.OUTSET=PIN3_bm;
	PORTD.DIRSET=PIN3_bm;                          //Pin TxD musi byæ wyjœciem
	USARTD0.CTRLA=USART_RXCINTLVL_LO_gc;           //Odblokuj przerwania odbiornika i nadajnika
	PMIC_CTRL|=PMIC_LOLVLEN_bm;                    //Odblokuj przerwanie niskiego poziomu
	USARTD0.CTRLB=USART_TXEN_bm | USART_RXEN_bm;   //W³¹cz nadajnik USART
	USARTD0.CTRLC=USART_CHSIZE_8BIT_gc;            //Ramka 8 bitów, bez parzystoœci, 1 bit stopu
	usart_set_baudrate(&USARTD0, 115200, F_CPU);     //Szybkoœæ transmisji
	
	//DMA_init();                                    //Zainicjuj DMA
}
void SendFullCommand(char* data)
{
	//cb_Send_Add(&sendBuf, data);
	USART_send(&USARTD0, data);
}
bool IsDataReady()
{
	return cmdrec>0;
}
uint8_t* ReceiveCommand()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)   //Licznik odebranych komend
	{
		if(cmdrec) --cmdrec;

	}

	CircBuffer* buf = &recBuf;
	
	char ch;
	for(uint8_t i=0; i<PARSE_BUFFER_SIZE; i++)        //Przepisz token do zmiennej parse_buffer w formacie NULLZ
	{
		if(cb_IsEmpty(buf)) return 0; //B³¹d
		ch=cb_Read(buf);
		if(ch=='\r' || ch=='\n') ch=0;	//THIS LINE - \n is REALLY IMPORTANT, Recompile and reflash it
		parse_buffer[i]=ch;
		if(ch==0) break;
	}
	
	return parse_buffer;
}

} //namespace
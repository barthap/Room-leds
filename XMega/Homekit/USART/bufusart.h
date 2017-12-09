/*
 * bufusart.h
 *
 * Created: 2013-01-24 18:12:26
 *  Author: tmf
 */ 


#ifndef BUFUSART_H_
#define BUFUSART_H_

#include "RingBuffer.h"

#define CB_SEND_MAXTRANS  40         //Maksymalna liczba elementów bufora

typedef struct
{
	uint8_t Beg;                       //Pierwszy element bufora
	uint8_t Count;                     //Liczba elementów w buforze
	char *elements[CB_SEND_MAXTRANS];  //Elementy bufora
} CircSendBuffer;

bool cb_Send_Add(CircSendBuffer *cb, char *elem);
char *cb_Send_Read(CircSendBuffer *cb);

static inline bool cb_Send_IsFull(CircSendBuffer *cb)
{
	return cb->Count == CB_SEND_MAXTRANS;
}

static inline bool cb_Send_IsEmpty(CircSendBuffer *cb)
{
	return cb->Count == 0;
}

extern CircBuffer recBuf;
extern CircSendBuffer sendBuf;

extern volatile uint8_t cmdrec;

void USART_send_buf_F(CircSendBuffer *buf, const char *txt);


#endif /* BUFUSART_H_ */
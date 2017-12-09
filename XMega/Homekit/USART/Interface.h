/* 
* Interface.h
*
* Created: 2014-04-22 16:23:26
* Author: Barthap
*/


#ifndef __INTERFACE_H__
#define __INTERFACE_H__

namespace USART
{
	#define PARSE_BUFFER_SIZE 20


	void Init();
	void SendFullCommand(char* data);
	uint8_t* ReceiveCommand();
	bool IsDataReady();
}



#endif //__INTERFACE_H__

/* 
* CommandSystem.h
*
* Created: 2014-04-22 17:29:59
* Author: Barthap
*/


#ifndef __COMMANDSYSTEM_H__
#define __COMMANDSYSTEM_H__

//adresy urzadzen
#define ADDR_MASTER 'A'
#define ADDR_BT		'B'
#define ADDR_PI		'C'

//numery porzadkowe bajtow
#define CMD_DEST 0
#define CMD_SENDER 1
#define CMD_ID 2
#define CMD_FIRST_ARG 3 //adres 1 argumentu

namespace CommandSystem
{
	void Update(); //wywo³ywane w pêtli g³ównej
}


#endif //__COMMANDSYSTEM_H__

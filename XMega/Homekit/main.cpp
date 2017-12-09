/*
 * Homekit.cpp
 *
 * Created: 22.10.2016 12:47:19
 * Author : Barthap
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "led_hw.h"
#include "led_fx.h"
#include "USART/Interface.h"
#include "CommandSystem.h"
#include "music.h"

#define SYSTEM_TC TCE1
#define SYSTEM_TC_INT TCE1_OVF_vect
#define SYSTEM_TC_PER 1250 //przy F_CPU=32MHz i prescalerze 256, ovf wychodzi co 0.01s = 10ms

volatile bool system_int_flag=false;
volatile uint16_t system_int_cnt=0;

inline void SystemTimer_Init()
{
	SYSTEM_TC.CTRLB=TC_WGMODE_NORMAL_gc;
	SYSTEM_TC.CTRLA=TC_CLKSEL_DIV256_gc;
	SYSTEM_TC.INTCTRLA=TC_OVFINTLVL_LO_gc;
	SYSTEM_TC.PER=SYSTEM_TC_PER;
}
ISR(SYSTEM_TC_INT) {
	if(system_int_flag)return;
	system_int_flag = true;
	system_int_cnt++;
}

void RC32MHz_enable()
{
	OSC.CTRL |= OSC_RC32MEN_bm;
	while(!(OSC.STATUS & OSC_RC32MEN_bm));
	CPU_CCP      =    CCP_IOREG_gc;         // odblokowanie zmiany èr¡d?a sygna?u
	CLK.CTRL     =    CLK_SCLKSEL_RC32M_gc; // zmiana èr¡d?a sygna?u na RC 32MHz
}

inline void SystemTimer_Update()
{
	if(!system_int_flag)return;
	system_int_flag = false;

	//do code here
	LED_FX::UpdateFades();
}


int main(void)
{
	RC32MHz_enable();

	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm; //W³¹czamy wszystkie poziomy przerwañ

	SystemTimer_Init();

	LED_HW::Init();
    LED_HW::AllOff();

	USART::Init(); //BAUD 115200

	sei();

	USART::SendFullCommand("Running\n");

    while (1) 
    {
		SystemTimer_Update();
		CommandSystem::Update();
//		filtered = calculateFilter(unfiltered);
    }
}


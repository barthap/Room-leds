/*
 * led_hw.cpp
 *
 * Created: 22.10.2016 16:20:09
 *  Author: Barthap
 */ 



 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include <avr/pgmspace.h>

 #include "led_hw.h"


 #if GAMMA_TABLE==05 //Gamma 0.7
 const uint8_t gamma_table[] PROGMEM = {
  5,  8, 11, 13, 16, 18, 20, 22, 24, 26, 28, 29, 31, 33, 34, 36, 38, 39,
  41, 42, 44, 45, 47, 48, 50, 51, 52, 54, 55, 56, 58, 59, 60, 62, 63, 64,
  65, 67, 68, 69, 70, 71, 73, 74, 75, 76, 77, 79, 80, 81, 82, 83, 84, 85,
  86, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,100,101,102,103,104,
  105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,
  123,124,125,126,127,128,129,130,131,132,132,133,134,135,136,137,138,139,
  140,141,142,142,143,144,145,146,147,148,149,150,150,151,152,153,154,155,
  156,156,157,158,159,160,161,162,162,163,164,165,166,167,167,168,169,170,
  171,172,172,173,174,175,176,177,177,178,179,180,181,181,182,183,184,185,
  185,186,187,188,189,189,190,191,192,193,193,194,195,196,196,197,198,199,
  200,200,201,202,203,203,204,205,206,206,207,208,209,210,210,211,212,213,
  213,214,215,216,216,217,218,219,219,220,221,221,222,223,224,224,225,226,
  227,227,228,229,230,230,231,232,232,233,234,235,235,236,237,238,238,239,
  240,240,241,242,243,243,244,245,245,246,247,247,248,249,250,250,251,252,
  252,253,254,255
 };
 #elif GAMMA_TABLE==20 //Gamma 2.0
 const uint8_t gamma_table[] PROGMEM = {
	 0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	1,	1,	1,
	 1,	1,	1,	2,	2,	2,	2,	2,	2,	3,	3,	3,	3,	4,	4,	4,
	 4,	4,	5,	5,	5,	6,	6,	6,	7,	7,	7,	8,	8,	8,	9,	9,
	 9,	10,	10,	11,	11,	11,	12,	12,	13,	13,	14,	14,	14,	15,	15,	16,
	 16,	17,	17,	18,	19,	19,	20,	20,	21,	21,	22,	22,	23,	24,	24,	25,
	 26,	26,	27,	27,	28,	29,	29,	30,	31,	32,	32,	33,	34,	34,	35,	36,
	 37,	37,	38,	39,	40,	40,	41,	42,	43,	44,	45,	45,	46,	47,	48,	49,
	 50,	51,	51,	52,	53,	54,	55,	56,	57,	58,	59,	60,	61,	62,	63,	64,
	 65,	66,	67,	68,	69,	70,	71,	72,	73,	74,	75,	76,	77,	78,	80,	81,
	 82,	83,	84,	85,	86,	88,	89,	90,	91,	92,	93,	95,	96,	97,	98,	100,
	 101,	102,	103,	105,	106,	107,	109,	110,
	 111,	112,	114,	115,	116,	118,	119,	121,
	 122,	123,	125,	126,	127,	129,	130,	132,
	 133,	135,	136,	138,	139,	140,	142,	143,
	 145,	146,	148,	149,	151,	153,	154,	156,
	 157,	159,	160,	162,	164,	165,	167,	168,
	 170,	172,	173,	175,	177,	178,	180,	182,
	 183,	185,	187,	188,	190,	192,	193,	195,
	 197,	199,	200,	202,	204,	206,	208,	209,
	 211,	213,	215,	217,	219,	220,	222,	224,
	 226,	228,	230,	232,	234,	235,	237,	239,
	 241,	243,	245,	247,	249,	251,	253,	255,
	 248,	250,	253,	255
 };
 #endif


 namespace LED_HW
 {

	 uint8_t curr[8];

	 uint8_t *Current() {return curr;}
	 
	 
	 void Init()
	 {
		 LED_PORT.DIRSET=0xFF;
		 LED_TIMER.CTRLE=TC_BYTEM_SPLITMODE_gc;
		 LED_TIMER.CTRLB=0xFF | TC2_HCMPB_bm;
		 LED_TIMER.CTRLA=TC_CLKSEL_DIV1_gc;
		 LED_TIMER.HPER=255;
		 LED_TIMER.LPER=255;
	 }

	 uint8_t Gamma(uint8_t val)
	 {
	 return val;
		// return (pgm_read_byte(&gamma_table[val]));
	 }

	 void Set(uint8_t channel, uint8_t val)
	 {
		//os³abienie zielonego przy ³ó¿ku bo za mocny
		 //if(channel==Bed_G && val>10)val=val-5;

		 switch(channel)
		 {
			 case 0: LED_TIMER.LCMPA = Gamma(curr[channel] = val); break;
			 case 1: LED_TIMER.LCMPB = Gamma(curr[channel] = val); break;
			 case 2: LED_TIMER.LCMPC = Gamma(curr[channel] = val); break;
			 case 3: LED_TIMER.LCMPD = Gamma(curr[channel] = val); break;
			 case 4: LED_TIMER.HCMPA = Gamma(curr[channel] = val); break;
			 case 5: LED_TIMER.HCMPB = Gamma(curr[channel] = val); break;
			 case 6: LED_TIMER.HCMPC = Gamma(curr[channel] = val); break;
			 case 7: LED_TIMER.HCMPD = Gamma(curr[channel] = val); break;
		 }
	 }
	 void Parse(uint8_t order, uint8_t values[8])
	 {
		 uint8_t index = 0;
		 if (order & 0x01) { Set(0, values[index]); index++; }
		 if (order & 0x02) { Set(1, values[index]); index++; }
		 if (order & 0x04) { Set(2, values[index]); index++; }
		 if (order & 0x08) { Set(3, values[index]); index++; }
		 if (order & 0x10) { Set(4, values[index]); index++; }
		 if (order & 0x20) { Set(5, values[index]); index++; }
		 if (order & 0x40) { Set(6, values[index]); index++; }
		 if (order & 0x80) { Set(7, values[index]);}
		 
		 ///if(index > 7) error
	 }
	 void AllOff()
	 {
		 uint8_t v[] = {0,0,0,0,0,0,0,0};
		 Parse(0xff,v);
	 }
	 void AllOn()
	 {
		 uint8_t v[] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
		 Parse(0xff,v);
	 }
 }
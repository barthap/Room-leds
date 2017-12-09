/*
 * led_hw.h
 *
 * Created: 22.10.2016 16:22:17
 *  Author: Barthap
 */ 


#ifndef LED_HW_H_
#define LED_HW_H_

//info o porcie modu?u
#define LED_PORT PORTC
#define LED_TIMER TCC2

#define GAMMA_TABLE 05

//nazwy kana?°w
#define LOZKO_R 7
#define LOZKO_G 6
#define LOZKO_B 5
#define LOZKO_W 4
#define SZAFA_W 3
#define POLKA_R 2
#define POLKA_G 1
#define POLKA_B 0

namespace LED_HW
{
	enum Channel
	{
		Bed_R = 7,
		Bed_G = 6,
		Bed_B = 5,
		Bookshelf = 3,
		Reserved = 4,
		Shelf_R = 2,
		Shelf_G = 1,
		Shelf_B = 0,
	};

	void AllOff();
	void AllOn();
	void Init();
	uint8_t *Current(); //zwraca wskaËnik na 1 elem. 8-bajtowej tablicy wartoùci PWM
	uint8_t Gamma(uint8_t val);
	void Set(uint8_t channel, uint8_t val);
	void Parse(uint8_t order, uint8_t values[8]);
}



#endif /* LED_HW_H_ */
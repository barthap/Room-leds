/*
 * led_fx.h
 *
 * Created: 23.10.2016 17:06:39
 *  Author: Barthap
 */ 


#ifndef LED_FX_H_
#define LED_FX_H_

struct Rgb
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	
	Rgb() : r(0), g(0), b(0) {}
	Rgb(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
};
struct Hsv
{
	uint8_t h;
	uint8_t s;
	uint8_t v;
	
	Hsv() : h(0), s(0), v(0) {}
	Hsv(uint8_t h, uint8_t s, uint8_t v) : h(h), s(s), v(v) {}
};

namespace LED_FX
{
	Rgb HsvToRgb(const Hsv& hsv);

	void AllOff();
	void AllOn();

	void SetFade(uint8_t channel, uint8_t val, uint8_t step=1);
	void Parse(uint8_t order, uint8_t values[8]);

	void UpdateFades();

}




#endif /* LED_FX_H_ */
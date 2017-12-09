/*
 * led_fx.cpp
 *
 * Created: 23.10.2016 17:06:30
 *  Author: Barthap
 */ 

 #include <avr/io.h>

 #include "led_hw.h"
 #include "led_fx.h"


 namespace LED_FX
 {

	uint8_t target[8];
	uint8_t steps[8] = {1,1,1,1, 1,1,1,1};
	
	void SetFade(uint8_t channel, uint8_t val, uint8_t step)
	{
		target[channel]=val;
		steps[channel]=step;
	}
	void UpdateFades()
	{
		uint8_t* curr = LED_HW::Current();

		for(uint8_t ch=0;ch<8;ch++)
		{
			if(curr[ch] > target[ch])
			{
				if(curr[ch]-target[ch]>=steps[ch])
					LED_HW::Set(ch,curr[ch]-steps[ch]);
				else 
					LED_HW::Set(ch,curr[ch]-1);
			}
			else if(curr[ch] < target[ch])
			{
				if(target[ch] - curr[ch] >= steps[ch])
					LED_HW::Set(ch, curr[ch]+steps[ch]);
				else
					LED_HW::Set(ch, curr[ch]+1);
			}
		}
	}
	
	void Parse(uint8_t order, uint8_t values[8])
	{
		uint8_t index = 0;
		if (order & 0x01) { SetFade(0, values[index]); index++; }
		if (order & 0x02) { SetFade(1, values[index]); index++; }
		if (order & 0x04) { SetFade(2, values[index]); index++; }
		if (order & 0x08) { SetFade(3, values[index]); index++; }
		if (order & 0x10) { SetFade(4, values[index]); index++; }
		if (order & 0x20) { SetFade(5, values[index]); index++; }
		if (order & 0x40) { SetFade(6, values[index]); index++; }
		if (order & 0x80) { SetFade(7, values[index]);}
		
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

	Rgb HsvToRgb(const Hsv& hsv)
	{
		
		Rgb rgb;
		unsigned char region, p, q, t;
		unsigned int h, s, v, remainder;

		if (hsv.s == 0)
		{
			rgb.r = hsv.v;
			rgb.g = hsv.v;
			rgb.b = hsv.v;

			return rgb;
		}

		// converting to 16 bit to prevent overflow
		h = hsv.h;
		s = hsv.s;
		v = hsv.v;

		region = h / 43;
		remainder = (h - (region * 43)) * 6;

		p = (v * (255 - s)) >> 8;
		q = (v * (255 - ((s * remainder) >> 8))) >> 8;
		t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

		switch (region)
		{
			case 0:
			rgb.r = v;
			rgb.g = t;
			rgb.b = p;
			break;
			case 1:
			rgb.r = q;
			rgb.g = v;
			rgb.b = p;
			break;
			case 2:
			rgb.r = p;
			rgb.g = v;
			rgb.b = t;
			break;
			case 3:
			rgb.r = p;
			rgb.g = q;
			rgb.b = v;
			break;
			case 4:
			rgb.r = t;
			rgb.g = p;
			rgb.b = v;
			break;
			default:
			rgb.r = v;
			rgb.g = p;
			rgb.b = q;
			break;
		}
		return rgb;
	}

 }
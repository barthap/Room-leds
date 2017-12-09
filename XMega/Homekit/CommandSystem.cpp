/* 
* CommandSystem.cpp
*
* Created: 2014-04-22 17:29:59
* Author: Barthap
*/
#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include "USART/Interface.h"
#include "CommandSystem.h"
#include "led_hw.h"
#include "led_fx.h"
#include "LED_Utils/hsv2rgb.h"
#include "LED_Utils/color.h"
#include "music.h"

extern bool randomLeds;

namespace FastLed {

static CRGB computeAdjustment(uint8_t scale, const CRGB & colorCorrection, const CRGB & colorTemperature) {
	#if defined(NO_CORRECTION) && (NO_CORRECTION==1)
	return CRGB(scale,scale,scale);
	#else
	CRGB adj(0,0,0);

	if(scale > 0) {
		for(uint8_t i = 0; i < 3; i++) {
			uint8_t cc = colorCorrection.raw[i];
			uint8_t ct = colorTemperature.raw[i];
			if(cc > 0 && ct > 0) {
				uint32_t work = (((uint32_t)cc)+1) * (((uint32_t)ct)+1) * scale;
				work /= 0x10000L;
				adj.raw[i] = work & 0xFF;
			}
		}
	}

	return adj;
	#endif
}

}

namespace CommandSystem
{
	char* buff;
	
	void Send(char* data)
	{
		USART::SendFullCommand((char*)data);
	}
	
	void TranslateCommands()
	{
		if(!buff)return; //nothing to do here
		
		if(strncmp(buff, "W=",2)==0)	//ustaw jasnoœæ White LED
		{
			char* pos;
			uint8_t val;
			pos = strchr(buff,'=')+1;
			val = atoi(pos);
			LED_FX::SetFade(LED_HW::Bookshelf, val);
		}
		else if(strncmp(buff, "B=",2)==0)	//ustaw jasnoœæ White LED
		{
			char* pos;
			Hsv hsv;
			Rgb rgb;
			pos = strchr(buff,'=')+1;
			hsv.v = atoi(pos);
			pos = strchr(buff,'h')+1;
			hsv.h = atoi(pos);
			pos = strchr(buff,'s')+1;
			hsv.s = atoi(pos);


			FastLed::CRGB crgb(0,0,0);
			FastLed::CHSV chsv(hsv.h,hsv.s,hsv.v);
			FastLed::hsv2rgb_rainbow(chsv,crgb);
			
			//rgb = LED_FX::HsvToRgb(hsv);

			#if defined(NO_CORRECTION)
			#undef NO_CORRECTION
			#endif

			crgb*=FastLed::computeAdjustment(1,FastLed::TypicalLEDStrip, FastLed::UncorrectedTemperature);

			LED_FX::SetFade(LED_HW::Bed_R, crgb.r);
			LED_FX::SetFade(LED_HW::Bed_G, crgb.g);
			LED_FX::SetFade(LED_HW::Bed_B, crgb.b);
		}
		else if(strncmp(buff, "S=",2)==0)	//ustaw jasnoœæ White LED
		{
			char* pos;
			Hsv hsv;
			Rgb rgb;
			pos = strchr(buff,'=')+1;
			hsv.v = atoi(pos);
			pos = strchr(buff,'h')+1;
			hsv.h = atoi(pos);
			pos = strchr(buff,'s')+1;
			hsv.s = atoi(pos);

			rgb = LED_FX::HsvToRgb(hsv);

			LED_FX::SetFade(LED_HW::Shelf_R, rgb.r);
			LED_FX::SetFade(LED_HW::Shelf_G, rgb.g);
			LED_FX::SetFade(LED_HW::Shelf_B, rgb.b);
		}
		else if(strncmp(buff, "M=",2)==0)	//RGB Music Mode
		{
			char* pos;
			Rgb rgb;
			pos = strchr(buff,'r')+1;
			rgb.r = atoi(pos);
			pos = strchr(buff,'g')+1;
			rgb.g = atoi(pos);
			pos = strchr(buff,'b')+1;
			rgb.b = atoi(pos);

			//unfiltered = rgb;
			rgb = calculateFilter(rgb);

			LED_FX::SetFade(LED_HW::Bed_R, rgb.r, 4);
			LED_FX::SetFade(LED_HW::Bed_G, rgb.g, 4);
			LED_FX::SetFade(LED_HW::Bed_B, rgb.b, 4);
		}
		else if(strncmp(buff, "Filter=",7)==0)
		{
			char* pos = strchr(buff, '=')+1;
			uint8_t val = atoi(pos);
			setFilter(val);
		}
		else if(buff[0]=='?')
		{
			_delay_ms(5);
			USART::SendFullCommand("OK\nTTTTTTTT\n\r");
		}
	}
	void Update()
	{
		buff = 0;
		if(USART::IsDataReady())
		{
			buff = (char*)USART::ReceiveCommand();
			TranslateCommands();
		}

					
	}
}
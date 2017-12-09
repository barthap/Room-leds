/*
 * music.cpp
 *
 * Created: 06.04.2017 18:45:15
 *  Author: Barthap
 */ 
 #include <avr/io.h>
 #include "led_fx.h"

 static uint8_t filter=2;

 static uint8_t r,g,b;
 static uint32_t sr,sg,sb;

 Rgb filtered, unfiltered;


Rgb calculateFilter(Rgb spectrum)
{
	if(filter == 0)return spectrum;

	sr = sr-r+spectrum.r;
	r=(sr+(1<<(filter-1))>>(filter));

	sg = sg-g+spectrum.g;
	g=(sg+(1<<(filter-1))>>(filter));
	
	sb = sb-b+spectrum.b;
	b=(sb+(1<<(filter-1))>>(filter));

	return Rgb(r,g,b);
}

void setFilter(uint8_t val)
{
	filter = val;
}
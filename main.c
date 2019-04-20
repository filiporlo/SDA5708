/*
 * main.c
 *
 *  Created on: 19 sie 2014
 *      Author: Filip Or³owski
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <string.h>

#include "SDA5708/sda5708.h"


const char napis1[] PROGMEM = "Pozdrawiam forum.atnel.pl filiporlo.blogspot.com";

const uint8_t strzalki[] PROGMEM = {
	0x73,0x9C,0xE7,0x39,0xCE,0x73,0x9C,0xE7,0x39,0xCE,0x73,0x9C,0xE7,0x39,0xCE,0x73,
	0x9C,0xE7,0x39,0xCE,0xFF,0xFF,0xFF,0xFF,0xFF,0x73,0x9C,0xE7,0x39,0xCE,0x21,0x08,
	0x42,0x10,0x84,
};

int main (void){

	sda5708_init();
	sda5708_brightness(14);
//	sda5708_drawBitmap_P(0, 0, picture, 40, 7, 1);
//	sda5708_display();



int8_t j=0;
//	int licznik=0;
	//sei();
	while(1){

		for(int8_t i=-7; i<7; i++){
			sda5708_drawBitmap_P(0, i, strzalki, 40, 7, 1);
			sda5708_display();
			sda5708_cls_buffer();
			_delay_ms(200);
		}

//		for(int8_t i=-1; i>-8; i--){
//			sda5708_drawBitmap_P(0, i, strzalki, 40, 7, 1);
//			sda5708_display();
//			sda5708_cls_buffer();
//			_delay_ms(200);
//		}

		for(int8_t i=DIGITS; i>-(int8_t)strlen(napis1); i--){
			sda5708_cls();
			sda5708_str_P( napis1, i);
			_delay_ms(300);
		}

		for(int8_t i=(int8_t)-strlen(napis1); i<DIGITS; i++){
			sda5708_cls();
			sda5708_str_P( napis1, i);
			_delay_ms(300);
		}

		for(j =0; j<20; j++){
			for(uint8_t i=0; i<7; i++){ sda5708_setPixel(j,i,1); sda5708_setPixel(39-j,i,1);}
			sda5708_display();
			_delay_ms(300);
		}


		for(j =19; j>-1; j--){
			for(uint8_t i=0; i<7; i++){ sda5708_setPixel(j,i,0); sda5708_setPixel(39-j,i,0);}
			sda5708_display();
			_delay_ms(300);
		}



	}
}

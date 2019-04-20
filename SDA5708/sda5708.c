/*
 * sda5708.c
 *
 *  Created on: 19 sie 2014
 *      Author: Filip Or�owski
 */
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>

#include "sda5708.h"
#include "font.h"

#if ONLY_GRAPHICS ==1
uint8_t sda5708_buf[BUF_SIZE];
#endif

uint8_t control = 0b11100000;	//domy�lne warto�ci rejestru odpowiadaj�cego za jasno�c i clr screen

//WYS�ANIE BAJTU DO SDA5708 OD LSB
static void sda5708_send( uint8_t data ){
	uint8_t i;

	LOAD_LO;	//zapis do sterownika

	for (i=0; i<=7;i++)	{

		if ((data>>i)&1) DATA_HI; else DATA_LO;

		SDCLK_HI;
		SDCLK_LO;
	}

	LOAD_HI;
}

//INICJALIZACJA SDA5708
void sda5708_init(void){
	LOAD_DDR |= LOAD;	//ustawienie kierunk�w lini wyj�ciowych SDA5708
	DATA_DDR |= DATA;
	SDCLK_DDR |= SDCLK;
	RESET_DDR |= RESET;

	RESET_LO;	//resetowanie uk�adu
	RESET_HI;

	LOAD_HI;
}

//USTAWIENIE SI�Y �WIECENIA WY�WIETLACZA
void sda5708_brightness( uint8_t power ){
	control = control|(power & 0b00001111);	//zapis do rejestru u�ywany przy sda5708_cls();
	sda5708_send(control);	//wys�anie danych
}

void sda5708_cls(void){
	sda5708_send(control&0b11011111);	//wys�anie polecenia z ustawiony bitem 5 clear screen
}

#if ONLY_TEXT == 1
//------------------------
//
//
//		CZʌC TEKSTOWA
//
//------------------------
void sda5708_char( uint8_t c, int8_t x ){
	if((x < 0) || (x > DIGITS-1)) return;	//warunek sprawdzaj�cy czy znak mie�ci si� na wy�wietlaczu

	if ((c < 0x20) || (c > 0x7b)) c = 0x20;	//je�eli znak nieznany zast�powany pustym polem spacja 0x20=32

	sda5708_send(0b10100000|x);	//wys�anie do sterownika informacji do kt�rej pozycji b�dziemy wysy�ac dane

	uint8_t i;
	for(i=0; i<7; i++){
		sda5708_send(pgm_read_byte(&font[(c-0x20)*7+i])>>3);	//pobranie z FLASH kolejno siedmiu linijek danego znaku
	}															//i przesuni�cie o 3 bity w prawo tak u�o�ona czcionka

}

//WYS�ANIE STRINGA Z RAM
void sda5708_str( char * str, int8_t digit ){
	int8_t i = digit;
	register char znak;
	while( (znak=*(str++)) ) sda5708_char( znak, i++);
}

//WYS�ANIE STRINGA Z FLASH
#if USE_SDA5708_STR_P == 1
void sda5708_str_P( const char * str, int8_t digit ){
		int8_t i = digit;
	register char znak;
	while( (znak=pgm_read_byte(str++)) ) sda5708_char( znak, i++);
}
#endif

//WY�WIETLA LICZB� DZIESI�TN�
#if USE_SDA5708_INT == 1
void sda5708_int( int val, int8_t digit ){
	char bufor[17];
		sda5708_str( itoa(val, bufor, 10), digit );
}
#endif

//WY�WIETLA LICZB� HEX
#if USE_SDA5708_HEX == 1
void sda5708_hex( uint32_t val, int8_t digit ){
	char bufor[17];
		sda5708_str( itoa(val, bufor, 16), digit );
}
#endif

#endif


#if ONLY_GRAPHICS == 1
//------------------------
//
//
//		CZʌC GRAFICZNA
//
//------------------------
//USTAWIENIE PIXELA W BUFORZE
void sda5708_setPixel( int8_t x, int8_t y, uint8_t color ){
	if(( x < 0) || (x > DIGITS*WIDTH-1) || (y < 0) || (y > HEIGHT-1)) return;

	if(color) sda5708_buf[ y + (x/WIDTH)*HEIGHT] |= (16>>x%5);	//ustawienie odpowiedniego bitu na 1 na odpowiedniej pozycji w tablicy
	else sda5708_buf[ y + (x/WIDTH)*HEIGHT] &= ~(16>>x%5);		//ustawienie odpowiedniego bitu na 0 na odpowiedniej pozycji w tablicy

}

//WYS�ANIE BUFORA DO SDA5708
void sda5708_display(void){
	for(uint8_t i=0; i<DIGITS; i++){
			sda5708_send(0b10100000|i);	//wys�anie do sterownika informacji do kt�rej pozycji b�dziemy wysy�ac dane
			for(uint8_t j=0; j<HEIGHT; j++){	//wys�anie danych bufor[6] odpowiada y=0
				sda5708_send(sda5708_buf[i*HEIGHT + j]);
			}
	}
}

//ZEROWANIE BUFORA
void sda5708_cls_buffer(void){
	memset( sda5708_buf, 0x00, BUF_SIZE);
}

void sda5708_drawBitmap_P( int8_t x, int8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color){
	int8_t i, j, byteWidth = (w + 7) / 8;
	for(j=0; j<h; j++){
		for(i=0; i<w; i++){
			if( pgm_read_byte( bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))){
				sda5708_setPixel(x+i, y+j, color);
			}
		}
	}
}

#endif

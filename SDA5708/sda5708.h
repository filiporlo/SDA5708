/*
 * sda5708.h
 *
 *  Created on: 19 sie 2014
 *      Author: Filip Or³owski
 *      Korzysta³em z http://www.sbprojects.com/knowledge/footprints/sda5708.php
 */

#ifndef SDA5708_H_
#define SDA5708_H_

//PIN FUNCTION OF SDA5708:
//	1	VCC
//	2	LOAD
//	3	DATA
//	4	SDCLK
//	5	RESET
//	6	GND


//DEFINICJE PREPROCESORA
#define LOAD	(1<<PC3)
#define DATA	(1<<PC1)
#define SDCLK	(1<<PC0)
#define RESET	(1<<PC2)

#define LOAD_PORT	PORTC
#define LOAD_DDR	DDRC

#define DATA_PORT	PORTC
#define DATA_DDR	DDRC

#define SDCLK_PORT	PORTC
#define SDCLK_DDR	DDRC

#define RESET_PORT	PORTC
#define RESET_DDR	DDRC

//USTAWIENIA KOMPILACJI
#define ONLY_TEXT			1
#define ONLY_GRAPHICS		1


#if ONLY_TEXT == 1
#define USE_SDA5708_STR_P	1
#define USE_SDA5708_INT		1
#define USE_SDA5708_HEX		0
#endif

#define DIGITS	8
#define HEIGHT 7
#define WIDTH 5

#define BUF_SIZE (DIGITS*HEIGHT)

//DEFINICJE DO "BANGLOWANIA" PINAMI
#define LOAD_LO		LOAD_PORT &= ~LOAD
#define LOAD_HI		LOAD_PORT |= LOAD

#define DATA_LO		DATA_PORT &= ~DATA
#define DATA_HI		DATA_PORT |= DATA

#define SDCLK_LO	SDCLK_PORT &= ~SDCLK
#define SDCLK_HI	SDCLK_PORT |= SDCLK

#define RESET_LO	RESET_PORT &= ~RESET
#define RESET_HI	RESET_PORT |= RESET


//DEKLARACJE FUNKCJI
void sda5708_init(void);
void sda5708_brightness( uint8_t power );
void sda5708_cls(void);

//CZÊŒC TEKSTOWA
void sda5708_char( uint8_t c, int8_t x );
void sda5708_str( char * str, int8_t digit );
void sda5708_str_P( const char * str, int8_t digit );
void sda5708_int( int val, int8_t digit );
void sda5708_hex( uint32_t val, int8_t digit );

//CZEŒC GRAFICZNA
void sda5708_setPixel( int8_t x, int8_t y, uint8_t color );
void sda5708_display(void);
void sda5708_cls_buffer(void);
void sda5708_drawBitmap_P( int8_t x, int8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color);


#endif /* SDA5708_H_ */

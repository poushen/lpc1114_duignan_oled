/** A oled i2c class partial porting from OLED_I2C library
 *
 *  @author  Poushen Ou
 *  @version 1.0
 *  @date    17-Jun-2018
 *
 *  This code provide operation for I2C OLED SSD1306
 *
 *  About OLED SSD1306
 *      https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
 */

/*
  OLED_I2C.h - Arduino/chipKit library support for 128x64 pixel SSD1306 OLEDs
  Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved

  This library has been made to make it easy to use 128x64 pixel OLED displays
  based on the SSD1306 controller chip with an Arduino or a chipKit.

  You can always find the latest version of the library at
  http://www.RinkyDinkElectronics.com/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the
  examples and tools supplied with the library.
*/
#ifndef _I2C_OLED_H_
#define _I2C_OLED_H_

#include "stdint.h"

#define LEFT		0
#define RIGHT		9999
#define CENTER	9998

#define SSD1306_128_64
//#define DEBUG_PRF1			// for debug print to uart

#define SSD1306_ADDR					0x3C
#define SSD1306_MEM_SIZE			360
#define SSD1306_WIDTH					72
#define SSD1306_LENGTH				40

#define SSD1306_COMMAND				0x00
#define SSD1306_DATA					0xC0
#define SSD1306_DATA_CONTINUE	0x40

//#define SSD1306_DATA_OUT_OPTIMIZED

// SSD1306 Commandset
// ------------------
// Fundamental Commands
#define SSD1306_SET_CONTRAST_CONTROL									0x81
#define SSD1306_DISPLAY_ALL_ON_RESUME									0xA4
#define SSD1306_DISPLAY_ALL_ON												0xA5
#define SSD1306_NORMAL_DISPLAY												0xA6
#define SSD1306_INVERT_DISPLAY												0xA7
#define SSD1306_DISPLAY_OFF														0xAE
#define SSD1306_DISPLAY_ON														0xAF
#define SSD1306_NOP																		0xE3
// Scrolling Commands
#define SSD1306_HORIZONTAL_SCROLL_RIGHT								0x26
#define SSD1306_HORIZONTAL_SCROLL_LEFT								0x27
#define SSD1306_HORIZONTAL_SCROLL_VERTICAL_AND_RIGHT	0x29
#define SSD1306_HORIZONTAL_SCROLL_VERTICAL_AND_LEFT		0x2A
#define SSD1306_DEACTIVATE_SCROLL											0x2E
#define SSD1306_ACTIVATE_SCROLL												0x2F
#define SSD1306_SET_VERTICAL_SCROLL_AREA							0xA3
// Addressing Setting Commands
#define SSD1306_SET_LOWER_COLUMN											0x00
#define SSD1306_SET_HIGHER_COLUMN											0x10
#define SSD1306_MEMORY_ADDR_MODE											0x20
#define SSD1306_SET_COLUMN_ADDR												0x21
#define SSD1306_SET_PAGE_ADDR													0x22
// Hardware Configuration Commands
#define SSD1306_SET_START_LINE												0x40
#define SSD1306_SET_SEGMENT_REMAP											0xA0
#define SSD1306_SET_MULTIPLEX_RATIO										0xA8
#define SSD1306_COM_SCAN_DIR_INC											0xC0
#define SSD1306_COM_SCAN_DIR_DEC											0xC8
#define SSD1306_SET_DISPLAY_OFFSET										0xD3
#define SSD1306_SET_COM_PINS													0xDA
#define SSD1306_CHARGE_PUMP														0x8D
// Timing & Driving Scheme Setting Commands
#define SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO						0xD5
#define SSD1306_SET_PRECHARGE_PERIOD									0xD9
#define SSD1306_SET_VCOM_DESELECT											0xDB

struct _current_font
{
	uint8_t* font;
	uint8_t x_size;
	uint8_t y_size;
	uint8_t offset;
	uint8_t numchars;
	uint8_t inverted;
};

extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

void comm_out(uint8_t a);
void comm_page_out(uint8_t* pPage, uint8_t size);
void data_out(uint8_t a);
void data_page_out(uint8_t* pPage, uint8_t size);
//void clear(void);
void init_oled(void);
void pixel_on(void);
void oled_test(uint8_t func);

void oled_clrScr(void);
void oled_fillScr(void);
//void oled_setBrightness(uint8_t value);
//void oled_invert(bool mode);
void oled_setPixel(uint16_t x, uint16_t y);
void oled_clrPixel(uint16_t x, uint16_t y);
//void oled_invertText(bool mode);
void oled_setFont(uint8_t* font);
void oled_print_char(unsigned char c, int x, int y);
void oled_print(char *st, int x, int y);
void oled_printNumI(long num, int x, int y, int length, char filler);
void oled_printNumF(double num, uint8_t dec, int x, int y, char divider, int length, char filler);
void oled_drawBitmap(int x, int y, uint8_t* bitmap, int sx, int sy);
void oled_update(void);

#endif

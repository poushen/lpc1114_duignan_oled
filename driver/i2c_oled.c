#include "i2c_eeprom.h"
#include "i2c_oled.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "ftoa.h"
#include "uart.h"

uint8_t scrbuf[360];
struct _current_font	cfont;

uint8_t ble_broadcast_small [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x40, 0xC0, 0x80, 0x00, 0x18, 0x38, 0x70, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xF0, 0xF8, 0xF8, 0xF8, 0xF8, 0xF0, 0x00, 0x00, 0x00, 0x04, 0x0C, 0x38, 0xF0, 0xC0,
0x03, 0x07, 0x3E, 0xF8, 0x80, 0x00, 0x03, 0x3E, 0xF8, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x1F, 0x3F, 0x3F, 0x3F, 0x3F, 0x1F, 0x00, 0x00, 0x00, 0x40, 0x60, 0x38, 0x1F, 0x07, 0x80, 0xC0,
0xF8, 0x3F, 0x03, 0x00, 0xC0, 0xF8, 0x3E, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x06, 0x03, 0x01, 0x30, 0x38,
0x1C, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
}; 

//uint8_t link_small [] = {
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x60, 0x70, 0xF8,
//0x7C, 0x3E, 0x1E, 0x0F, 0x0F, 0x0F, 0x0F, 0x1E, 0xFE, 0xFC, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x80,
//0xC0, 0xE0, 0xF0, 0xF8, 0x7C, 0x3E, 0x9E, 0xCE, 0xCE, 0x8E, 0x9E, 0x9E, 0x9C, 0xC8, 0xE0, 0xF0,
//0xF8, 0x7C, 0x3E, 0x1F, 0x0F, 0x07, 0x03, 0x00, 0x78, 0xFE, 0xFF, 0xFF, 0x87, 0x03, 0x01, 0x00,
//0x80, 0xC0, 0xE1, 0xF3, 0xF3, 0x77, 0x27, 0x07, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x01,
//0x00, 0x00,
//};

void comm_out(uint8_t a)
{
		i2c_eeprom_byte_write(SSD1306_ADDR, SSD1306_COMMAND, a);

#ifdef DEBUG_PRF1
		uartPutHex(a); uartPuts("\n");
#endif
}

void comm_page_out(uint8_t* pPage, uint8_t size)
{
		i2c_eeprom_page_write(SSD1306_ADDR, SSD1306_COMMAND, pPage, size);
}

void data_out(uint8_t a)
{
		i2c_eeprom_byte_write(SSD1306_ADDR, SSD1306_DATA_CONTINUE, a);
}

void data_page_out(uint8_t* pPage, uint8_t size)
{
		i2c_eeprom_page_write(SSD1306_ADDR, SSD1306_DATA_CONTINUE, pPage, size);
}

//void clear(void)
//{
//		int i,j;
//		for(i=0;i<8;i++)
//		{
//				comm_out(0xb0+i);
//				comm_out(0x00);
//				comm_out(0x10);
//				for(j=0;j<132;j++)
//				{
//						data_out(0x00);
//				}
//		}
//}

void init_oled(void)
{
#ifdef SSD1306_128_64
		uint8_t comm[30] = {SSD1306_DISPLAY_OFF,
												SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO,
												0x80,
												SSD1306_SET_MULTIPLEX_RATIO,
												0x3F,
												SSD1306_SET_DISPLAY_OFFSET,
												0x00,
												SSD1306_SET_START_LINE,
												SSD1306_CHARGE_PUMP,
												0x14,
												SSD1306_MEMORY_ADDR_MODE,
												0x00,				// Horizontal Addressing Mode
						// --------
												0x21,				// Set Column Address
												0x00,		//28
												127,
												0x22,				// Set Page Address
												0,
												7,
						// --------
												SSD1306_SET_SEGMENT_REMAP | 0x1,
												SSD1306_COM_SCAN_DIR_DEC,
												SSD1306_SET_COM_PINS,
												0x12,
//												0xAD,
//												0x30,
												SSD1306_SET_CONTRAST_CONTROL,
												0xCF,
												SSD1306_SET_PRECHARGE_PERIOD,
												0xF1,
												SSD1306_SET_VCOM_DESELECT,
												0x40,
												SSD1306_DISPLAY_ALL_ON_RESUME,
												SSD1306_NORMAL_DISPLAY,};
		comm_page_out(comm, 30);
#else
		uint8_t comm[32] = {SSD1306_DISPLAY_OFF,
												SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO,
												0xF0,
												SSD1306_SET_MULTIPLEX_RATIO,
												0x27,
												SSD1306_SET_DISPLAY_OFFSET,
												0x00,
												SSD1306_SET_START_LINE,
												SSD1306_CHARGE_PUMP,
												0x95,
												SSD1306_MEMORY_ADDR_MODE,
												0x00,				// Horizontal Addressing Mode
						// --------
												0x21,				// Set Column Address
												0x1c,
												99,
												0x22,				// Set Page Address
												0,
												4,
						// --------
												SSD1306_SET_SEGMENT_REMAP | 0x1,
												SSD1306_COM_SCAN_DIR_DEC,
												SSD1306_SET_COM_PINS,
												0x12,
												0xAD,
												0x30,
												SSD1306_SET_CONTRAST_CONTROL,
												0xFF,
												SSD1306_SET_PRECHARGE_PERIOD,
												0x22,
												SSD1306_SET_VCOM_DESELECT,
												0x20,
												SSD1306_DISPLAY_ALL_ON_RESUME,
												SSD1306_NORMAL_DISPLAY,};
		comm_page_out(comm, 32);
#endif

#ifdef SSD1306_128_64
		for(int i=0; i<8; i++) {
			uint8_t data[3] = {0xb0+i, 0x00, 0x00};
			comm_page_out(data, 3);

			for(int j=0; j<128; j++)
				data_out(0x00);
		}
#else
		oled_clrScr();
		oled_update();
#endif
		comm_out(SSD1306_DISPLAY_ON); 									//Set Display On

#ifdef DEBUG_PRF1
	uartPuts("\n");
	uartPuts("---> Enter init_oled --- \n");
#endif

	// comm_out(SSD1306_DISPLAY_OFF);
	// comm_out(SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO);
	// comm_out(0x80);
	// comm_out(SSD1306_SET_MULTIPLEX_RATIO);
	// comm_out(0x3F);
	// comm_out(SSD1306_SET_DISPLAY_OFFSET);
	// comm_out(0x00);
	// comm_out(SSD1306_SET_START_LINE);
	// comm_out(SSD1306_CHARGE_PUMP);
	// comm_out(0x14);
	// comm_out(SSD1306_MEMORY_ADDR_MODE);
	// comm_out(0x00);						// Horizontal Addressing Mode
	// comm_out(0x21);						// Set Column Address
	// comm_out(0x00);
	// comm_out(127);
	// comm_out(0x22);						// Set Page Address
	// comm_out(0);
	// comm_out(7);

	// comm_out(SSD1306_SET_SEGMENT_REMAP | 0x1);
	// comm_out(SSD1306_COM_SCAN_DIR_DEC);
	// comm_out(SSD1306_SET_COM_PINS);
	// comm_out(0x12);
	// comm_out(0xAD);
	// comm_out(0x30);
	// comm_out(SSD1306_SET_CONTRAST_CONTROL);
	// comm_out(0xCF);
	// comm_out(SSD1306_SET_PRECHARGE_PERIOD);
	// comm_out(0xF1);
	// comm_out(SSD1306_SET_VCOM_DESELECT);
	// comm_out(0x40);
	// comm_out(SSD1306_DISPLAY_ALL_ON_RESUME);
	// comm_out(SSD1306_NORMAL_DISPLAY);

	// for(int i=0; i<8; i++) {
		// uint8_t data[3] = {0xb0+i, 0x00, 0x00};
		// comm_out(data[0]);
		// comm_out(data[1]);
		// comm_out(data[2]);

		// for(int j=0; j<128; j++)
			// data_out(0x00);
	// }

	// comm_out(SSD1306_DISPLAY_ON); 									//Set Display On
}

void pixel_on(void)
{
#ifdef SSD1306_128_64
		for(int i=0; i<5; i++)
		{
				uint8_t data[3] = {0xb2+i, 0x0c, 0x11};
				comm_page_out(data, 3);

				data_page_out(&scrbuf[i*SSD1306_WIDTH], 30);
				data_page_out(&scrbuf[i*SSD1306_WIDTH + 30], 30);
				data_page_out(&scrbuf[i*SSD1306_WIDTH + 60], 12);
		}
#else
		uint8_t data[3] = {0xb0, 0x0c, 0x11};
		comm_page_out(data, 3);

		for(int i=0; i<11; i++)
		{
				data_page_out(&scrbuf[i*32], 32);
		}
		data_page_out(&scrbuf[352], 8);
#endif

//		for(int i=0;i<5;i++)
//		{
//				comm_out(0xb0+i);
//				comm_out(0x0c);
//				comm_out(0x11);
//
//				for(int j=0;j<72;j++)
//				{
//						data_out(scrbuf[i*SSD1306_WIDTH + j]);
//				}
//		}

}


void oled_update(void)
{
		pixel_on();
}

void oled_test(uint8_t func)
{
		switch (func) {
			case 0:
				oled_clrScr();
				oled_drawBitmap(20, 5, ble_broadcast_small, 30, 32);
				oled_update();
				break;
			case 1:
				oled_clrScr();
				oled_setFont(SmallFont);
				oled_print("---Connected---", CENTER, 17);
				//oled_drawBitmap(24, 6, link_small, 28, 28);
				oled_update();
				break;
			case 2:
				oled_setFont(SmallFont);
				oled_clrScr();
				oled_print("Incoming", CENTER, 0);
				oled_print("Call", CENTER, 8);
				oled_print("0921868475", CENTER, 24);
				oled_update();
				break;
			case 3:
				oled_setFont(MediumNumbers);
				oled_clrScr();
				oled_print("0921", CENTER, 0);
				oled_print("868475", CENTER, 20);
				oled_update();
				break;
			case 4:
				oled_setFont(BigNumbers);
				oled_clrScr();
				oled_print("27", 20, 10);
				oled_setFont(SmallFont);
				oled_print("~c", 50, 24);
				oled_update();
				break;

			case 11:
				//clear();
				oled_clrScr();
				oled_update();
				break;
		}
		}

void oled_clrScr()
{
		memset(scrbuf, 0, SSD1306_MEM_SIZE);
}

void oled_fillScr()
{
		memset(scrbuf, 0xFF, SSD1306_MEM_SIZE);
}

//void oled_setBrightness(uint8_t value)
//{
//		comm_out(SSD1306_SET_CONTRAST_CONTROL);
//		comm_out(value);
//}

//void oled_invert(bool mode)
//{
//		if (mode)
//				comm_out(SSD1306_INVERT_DISPLAY);
//		else
//				comm_out(SSD1306_NORMAL_DISPLAY);
//}

void oled_setPixel(uint16_t x, uint16_t y)
{
		int by, bi;

		if ((x<SSD1306_WIDTH) && (y<SSD1306_LENGTH))
		{
				by = ((y/8)*SSD1306_WIDTH) + x;
				bi = y % 8;

				scrbuf[by] = scrbuf[by] | (1<<bi);
		}
}

void oled_clrPixel(uint16_t x, uint16_t y)
{
		int by, bi;

		if ((x<SSD1306_WIDTH) && (y<SSD1306_LENGTH))
		{
				by = ((y/8)*SSD1306_WIDTH) + x;
				bi = y % 8;

				scrbuf[by] = scrbuf[by] & ~(1<<bi);
		}
}

//void oled_invertText(bool mode)
//{
//		if (mode)
//				cfont.inverted = 1;
//		else
//				cfont.inverted = 0;
//}

void oled_setFont(uint8_t* font)
{
		cfont.font = font;
		cfont.x_size = font[0];
		cfont.y_size = font[1];
		cfont.offset = font[2];
		cfont.numchars = font[3];
		//cfont.inverted = 0;
}

void oled_print_char(unsigned char c, int x, int y)
{
#ifdef DEBUG_PRF1
	uartPuts("enter oled print char\n");
#endif
		if ((cfont.y_size % 8) == 0)
		{
				int font_idx = ((c - cfont.offset)*(cfont.x_size * (cfont.y_size/8)))+4;
				for (int rowcnt=0; rowcnt<(cfont.y_size/8); rowcnt++)
				{
					for(int cnt=0; cnt<cfont.x_size; cnt++)
					{
						for (int b=0; b<8; b++)
							if ((cfont.font[font_idx+cnt+(rowcnt*cfont.x_size)] & (1<<b))!=0)
								//if (cfont.inverted==0)
									oled_setPixel(x+cnt, y+(rowcnt*8)+b);
								//else
								//	oled_clrPixel(x+cnt, y+(rowcnt*8)+b);
							else
								//if (cfont.inverted==0)
									oled_clrPixel(x+cnt, y+(rowcnt*8)+b);
								//else
								//	oled_setPixel(x+cnt, y+(rowcnt*8)+b);
					}
				}
		}
		else
		{
			int font_idx = ((c - cfont.offset)*((cfont.x_size*cfont.y_size/8)))+4;
			int cbyte=cfont.font[font_idx];
			int cbit=7;
			for (int cx=0; cx<cfont.x_size; cx++)
			{
				for (int cy=0; cy<cfont.y_size; cy++)
				{
					if ((cbyte & (1<<cbit)) != 0)
						//if (cfont.inverted==0)
							oled_setPixel(x+cx, y+cy);
						//else
						//	oled_clrPixel(x+cx, y+cy);
					else
						//if (cfont.inverted==0)
							oled_clrPixel(x+cx, y+cy);
						//else
						//	oled_setPixel(x+cx, y+cy);
					cbit--;
					if (cbit<0)
					{
						cbit=7;
						font_idx++;
						cbyte=cfont.font[font_idx];
					}
				}
			}
		}
}

void oled_print(char *st, int x, int y)
{
#ifdef DEBUG_PRF1
	uartPuts("enter oled print\n");
#endif
		//unsigned char ch;
		int stl;

		stl = strlen(st);
#ifdef DEBUG_PRF1
	uartPuts("after strlen call\n");
#endif
		if (x == RIGHT)
				x = SSD1306_WIDTH - (stl * cfont.x_size);
		if (x == CENTER)
				x = (SSD1306_WIDTH - (stl * cfont.x_size))/2;

		for (int cnt=0; cnt<stl; cnt++)
				oled_print_char(*st++, x + (cnt*(cfont.x_size)), y);
}

void oled_printNumI(long num, int x, int y, int length, char filler)
{
	char buf[25];
	char st[27];
	bool neg=false;
	int c=0, f=0;

	if (num==0)
	{
		if (length!=0)
		{
			for (c=0; c<(length-1); c++)
				st[c]=filler;
			st[c]=48;
			st[c+1]=0;
		}
		else
		{
			st[0]=48;
			st[1]=0;
		}
	}
	else
	{
		if (num<0)
		{
			neg=true;
			num=-num;
		}

		while (num>0)
		{
			buf[c]=48+(num % 10);
			c++;
			num=(num-(num % 10))/10;
		}
		buf[c]=0;

		if (neg)
		{
			st[0]=45;
		}

		if (length>(c+neg))
		{
			for (int i=0; i<(length-c-neg); i++)
			{
				st[i+neg]=filler;
				f++;
			}
		}

		for (int i=0; i<c; i++)
		{
			st[i+neg+f]=buf[c-i-1];
		}
		st[c+neg+f]=0;

	}

	oled_print(st,x,y);
}

void oled_printNumF(double num, uint8_t dec, int x, int y, char divider, int length, char filler)
{
	char st[27];
	bool neg=false;

	if (num<0)
		neg = true;

	//_convert_float(st, num, length, dec);
	ftoa(num, st, dec);

	if (divider != '.')
	{
		for (int i=0; i<sizeof(st); i++)
			if (st[i]=='.')
				st[i]=divider;
	}

	if (filler != ' ')
	{
		if (neg)
		{
			st[0]='-';
			for (int i=1; i<sizeof(st); i++)
				if ((st[i]==' ') || (st[i]=='-'))
					st[i]=filler;
		}
		else
		{
			for (int i=0; i<sizeof(st); i++)
				if (st[i]==' ')
					st[i]=filler;
		}
	}

	oled_print(st,x,y);
}

void oled_drawBitmap(int x, int y, uint8_t* bitmap, int sx, int sy)
{
	int bit;
	uint8_t data;

	for (int cy=0; cy<sy; cy++)
	{
		bit = cy % 8;
		for(int cx=0; cx<sx; cx++)
		{
			data = bitmap[cx+((cy/8)*sx)];
			if ((data & (1<<bit))>0)
				oled_setPixel(x+cx, y+cy);
			else
				oled_clrPixel(x+cx, y+cy);
		}
	}
}

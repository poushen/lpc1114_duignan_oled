//
// main.c - a example for ARM LPC1114
// Written by Ted Burke (based on code by Frank Duignan)
// Last updated 29-11-2013
//

#include "lpc111x.h"
#include <stdint.h>
#include "led.h"
#include "uart.h"
#include "systick.h"
#include "i2c_eeprom.h"
#include "i2c_oled.h"

#define BUFFER_SIZE 64

int main()
{
    uint8_t read_buffer[BUFFER_SIZE];

    ledCfg();
    uartCfg();
    i2cConfig();
	SysTickCfg(48000); // let SysTick interrupt at 1MHz

	ledOn();
	uartPuts("\n*************\n");
	uartPuts("*  I2C Test *\n");
	uartPuts("*************\n");
	
	ADDRESS_SIZE = ONE_BYTE_ADDRESS;
	init_oled();
	oled_test(0);
	
	delay_ms(1000);
	oled_test(1);
	
	delay_ms(1000);
	oled_test(2);
	
	delay_ms(1000);
	oled_test(3);
	
	delay_ms(1000);
	oled_test(4);
	
	ADDRESS_SIZE = TWO_BYTES_ADDRESS;
    // ---------- byte write ------------------
  	uartPuts("write to address: "); uartPutDec(12288); uartPuts("\n");
  	for (int i=0; i<64; i++) {
  		i2c_eeprom_byte_write(SLAVE_ADDRESS, 12288+i, i);
  		delay_ms(5);
  		i2c_eeprom_ack_polling(SLAVE_ADDRESS);
  	}

  	// ---------- SEQUENTIAL READ -------------
  	i2c_eeprom_write_address(SLAVE_ADDRESS, 12288);
  	i2c_eeprom_sequential_read(SLAVE_ADDRESS, read_buffer, 64);

  	for (int i=0; i<64; i++) {
  		uartPutHex((uint8_t)read_buffer[i]); uartPuts(" ");
  	}
  	uartPuts("\n");
	
	ledOff();
	while (1);

}

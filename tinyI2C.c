//
// ATtiny13 用 I2C ライブラリ tinyI2Clib for Atmel Studio by hunamizawa
//
// たま吉さん作の Arduino IDE 向けコードを Atmel Studio へ移植したものです
// 元のソースは http://nuneno.cocolog-nifty.com/blog/attiny13a/index.html にあります
//
// 2014/11/04 作成 by たま吉さん
// 2018/04/15 I2C_DATA_HI(),I2C_DATA_LO(),I2C_Stop(),I2C_Init(),ディレイ時間の修正 by たま吉さん
// 2019/03/26 移植 by hunamizawa
//
//  参考HP
//   オリジナルソース I2C on an AVR using bit banging
//    http://codinglab.blogspot.jp/2008/10/i2c-on-avr-using-bit-banging.html
//   オリジナル応用例 bvdeenen/arduino-ethernet-tmp102
//    https://github.com/bvdeenen/arduino-ethernet-tmp102/blob/master/temp_humidity.ino
//

//#include "F_CPU.h"
#include <avr/io.h>
#include <util/delay.h>
#include "tinyI2C.h"

// Port for the I2C
#define I2C_DDR 	DDRB
#define I2C_PIN 	PINB
#define I2C_PORT 	PORTB

// Pins to be used in the bit banging
#define I2C_CLK     PORTB3
#define I2C_DAT     PORTB4

#define I2C_DATA_HI()   I2C_DDR &= ~(1<<I2C_DAT)
#define I2C_DATA_LO()   I2C_DDR |= (1<<I2C_DAT)
#define I2C_CLOCK_HI()  I2C_DDR &= ~(1 << I2C_CLK); I2C_PORT |= (1 << I2C_CLK)
#define I2C_CLOCK_LO()  I2C_DDR |= (1 << I2C_CLK); I2C_PORT &= ~(1 << I2C_CLK)
#define RWWAIT 10

void I2C_WriteBit(uint8_t c) {
	if (c > 0) {
		I2C_DATA_HI();
	} else {
		I2C_DATA_LO();
	}
	I2C_CLOCK_HI();
	while ((I2C_PIN & (1 << I2C_CLK)) == 0) ;
	_delay_us(RWWAIT);
	I2C_CLOCK_LO();
	_delay_us(RWWAIT);
	if (c > 0) {
		I2C_DATA_LO();
	}
	_delay_us(RWWAIT);
}

uint8_t I2C_ReadBit() {
	I2C_DATA_HI();
	I2C_CLOCK_HI();
	while ((I2C_PIN & (1 << I2C_CLK)) == 0) ;
	_delay_us(RWWAIT);
	uint8_t c = I2C_PIN;
	I2C_CLOCK_LO();
	_delay_us(RWWAIT);
	return (c >> I2C_DAT) & 1;
}

// Inits bitbanging port, must be called before using the functions below
//
void I2C_Init() 
{
	I2C_DDR &= ~((1 << I2C_CLK) | (1 << I2C_DAT)); //bus pulled hi by pullup resistor.
	I2C_PORT &= ~((1 << I2C_CLK) | (1 << I2C_DAT)); // lo when output, HI-Z when input. NEVER CHANGE THIS.
	I2C_CLOCK_HI();
	I2C_DATA_HI();
}

// Send a START Condition
//
void I2C_Start() {
	// set both to high at the same time
	I2C_DDR &= ~((1 << I2C_DAT) | (1 << I2C_CLK));
	_delay_us(RWWAIT);
	I2C_DATA_LO();
	_delay_us(RWWAIT);
	I2C_CLOCK_LO();
	_delay_us(RWWAIT);
}

// Send a STOP Condition
//
void I2C_Stop(){
	I2C_CLOCK_LO();
	I2C_DATA_LO();

	I2C_CLOCK_HI();
	_delay_us(RWWAIT);
	I2C_DATA_HI();
	_delay_us(RWWAIT);
}

// write a byte to the I2C slave device
//
void I2C_Write(uint8_t c) {
	for (char i = 0; i < 8; i++) {
		I2C_WriteBit(c & 0x80);
		c <<= 1;
	}
}

// read a byte from the I2C slave device
//
uint8_t I2C_Read(uint8_t ack) {
	uint8_t res = 0;
	for (char i = 0; i < 8; i++) {
		res <<= 1;
		res |= I2C_ReadBit();
	}
	if (ack > 0) {
		I2C_WriteBit(0);
		} else {
		I2C_WriteBit(1);
	}
	return res;
}

//
// ATtiny13 用 I2C ライブラリ tinyI2Clib for Atmel Studio by hunamizawa
// たま吉さん作の Arduino IDE 向けコードを Atmel Studio へ移植したものです
// 元のソースは http://nuneno.cocolog-nifty.com/blog/attiny13a/index.html にあります
//
// 2019/03/26 移植に伴いヘッダファイル作成 by hunamizawa
//

#ifndef TINYI2C_H_
#define TINYI2C_H_

void I2C_WriteBit(uint8_t c);

uint8_t I2C_ReadBit();

// Inits bitbanging port, must be called before using the functions below
void I2C_Init();

// Send a START Condition
void I2C_Start();

// Send a STOP Condition
void I2C_Stop();

// write a byte to the I2C slave device
void I2C_Write(uint8_t c);

// read a byte from the I2C slave device
uint8_t I2C_Read(uint8_t ack);

#endif /* TINYI2C_H_ */

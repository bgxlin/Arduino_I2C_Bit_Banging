/* Read/Write AT24C32 EEPROM via software I2C
 * Features: Random Byte Read, Random Byte Write
 * Benjamin Lin 20190901
 *
 * TODO: Page read/write
 * 
 * References: 
 * 1. Ionescu, Eugen. AN1488: Bit Banging I2C™ on Mid-Range MCUs with the XC8 C Compiler.  Microchip.
 * 
 */

#include <stdint.h>
#include "tinyI2C.h"

#define LOBYTE(w) ((uint8_t)(w))
#define HIBYTE(w) ((uint8_t)(((uint16_t)(w) >> 8) & 0xFF))

class BB_AT24C32
{
private:
  uint8_t _read_address = 0;
  uint8_t _write_address = 0;

public:
  const uint16_t SIZE = 0x1000; // 0x0000..0x0FFF for AT24C32, 4KB 
  
public:
  void begin(uint8_t read_address, uint8_t write_address)
  {
    _read_address = read_address;
    _write_address = write_address;
  }

  uint8_t write_byte(uint16_t address, uint8_t data)
  {
    I2C_Start();

    // Write Control Byte
    I2C_Write(_write_address);
    if (I2C_ReadBit() != 0)
    {
      I2C_Stop();
      return 1;  // failed to write control byte, likely wrong I2C address
    }
    
    I2C_Write(HIBYTE(address));
    if (I2C_ReadBit() != 0)
    {
      I2C_Stop();
      return 2; // failed to send HIGH address
    }
    
    I2C_Write(LOBYTE(address));
    if (I2C_ReadBit() != 0)
    {
      I2C_Stop();
      return 3; // failed to send LOW address
    }
    
    I2C_Write(data);
    for (int i = 0; i < 100; i++)
    {
      if (I2C_ReadBit() ==0)
      {
        I2C_Stop();
        return 0;  // finally, data is written to EEPROM
      }
    }
    I2C_Stop();
    return 5;  // timeout receiving ACK from EEPROM
  }

  uint8_t read_byte(uint16_t address, uint8_t* data)
  {
    I2C_Start();

    // Write Control Byte
    I2C_Write(_write_address);
    if (I2C_ReadBit() != 0)
    {
      I2C_Stop();
      return 1; // failed to write control byte, likely wrong I2C address
    }

    I2C_Write(HIBYTE(address));
    if (I2C_ReadBit() != 0)
    {
      I2C_Stop();
      return 2; // failed to send HIGH address
    }
    
    I2C_Write(LOBYTE(address));
    if (I2C_ReadBit() != 0)
    {
      I2C_Stop();
      return 3; // failed to send LOW address
    }

    I2C_Start();
    I2C_Write(_read_address);
    if (I2C_ReadBit() != 0)
    {
      I2C_Stop();
      return 4; // failed to write control byte
    }
    
    *data = I2C_Read(0);  // no ack
    // NACK
    I2C_WriteBit(1);
    
    I2C_Stop();
    return 0;
  }
};

/* Software I2C with AT24C32
 * The memory chip tested is part of RTC3231, a real time clock module based on DS3231.
 * I2C Address: MSB-> 1 0 1 0 A2 A1 A0 R/W <-LSB  (R: 1; W: 0)
 * Wiring:
 *     UNO pin 11 - SCL
 *     UNO pin 12 - SDA
 * Memory & Time:
 *     UNO - 3124 bytes Flash, 342 bytes RAM. 46051ms write time, 6032ms read time.
 *     ATtiny13 (No Serial, Compile only, untested) - 544 bytes Flash, 5 bytes RAM.
 */

#include "tinyI2C.c"
#include "BB_AT24C32.cpp" 

#define I2C_ADDR_READ   0xAF
#define I2C_ADDR__WRITE 0xAE

void setup()
{
  Serial.begin(9600);
  Serial.println("I2C Bit Banging EEPROM test."); Serial.flush();
  
  pinMode(LED_BUILTIN, OUTPUT);

  I2C_Init();

  BB_AT24C32 eeprom = BB_AT24C32();
  eeprom.begin(I2C_ADDR_READ, I2C_ADDR__WRITE);

  uint8_t status_code;
  uint8_t data;
  uint8_t i = 0x0000;
  unsigned long start_ts = millis();
  for (uint16_t i = 0x0000; i < eeprom.SIZE; i++)
  {
    data = 'A' + (i % 25);  // write 'A'..'Y'
    status_code = eeprom.write_byte(i, data);
    if (status_code != 0)
    {
      Serial.print("Write failed with status code "); Serial.print(status_code); Serial.print(" at address "); Serial.println(i); Serial.flush();
      break;
    }
    if ((i >0) && (i % 512 == 0))
    {
      Serial.print("Wrote "); Serial.print(i); Serial.println(" bytes."); Serial.flush();
    }
    delay(10);
  }
  Serial.print("Write status code: "); Serial.println(status_code); Serial.flush();
  unsigned long elapsed_time = millis() - start_ts;
  Serial.print("Write time: "); Serial.print(elapsed_time); Serial.println("ms"); Serial.flush();
 
  start_ts = millis();
  for (uint16_t i = 0x0000; i < eeprom.SIZE; i++)
  {
    status_code = eeprom.read_byte(i, &data);
    if (status_code == 0)
    {
      Serial.print((char)data);
      if ((i+1) % 100 == 0)
        Serial.println("");
    }
    else 
    {
      Serial.print("Read status code: "); Serial.println(status_code); Serial.flush();
      break;
    }
  }
  Serial.println(""); Serial.flush();
  elapsed_time = millis() - start_ts;
  Serial.print("Read time: "); Serial.print(elapsed_time); Serial.println("ms"); Serial.flush();
  
}

void loop()
{
  delay(1000);  // nothing to do in this test setup.
}

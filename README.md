# Arduino_I2C_Bit_Banging
Read/Write AT24C32 EEPROM via software I2C.

 * The memory chip tested is part of RTC3231, a real time clock module based on DS3231.
 * I2C Address: MSB-> 1 0 1 0 A2 A1 A0 R/W <-LSB  (R: 1; W: 0)
 * Wiring:
 *     UNO pin 11 - SCL
 *     UNO pin 12 - SDA
 * Memory & Time:
 *     UNO - 3124 bytes Flash, 342 bytes RAM. 46051ms write time, 6032ms read time.
 *     ATtiny13 (No Serial, Compile only, untested) - 544 bytes Flash, 5 bytes RAM.

/**
 * Perilib Peripheral Device Library Example Code
 * Copyright (c) 2020 Jeff Rowberg
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software 
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED *AS IS*, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
 
#include <Perilib.h>

uint8_t rxBuffer[512];
uint8_t txBuffer[512];
Perilib::TwiRegisterInterface_ArduinoWire twiSlave(0x04, &Wire);

void setup() {
  // initialize host serial interface for monitoring
  Serial.begin(9600);
  
  // wait for enumeration with USB-based serial interface (optional)
  //while (!Serial);
  
  // initialize Wire interface
  Wire.begin();

  // initialize TX buffer with recognizable pattern
  int i;
  for (i = 0; i < 512; i++) txBuffer[i] = i & 0xFF;
}

void loop() {
  int bytesProcessed;
  
  uint8_t u8;
  uint16_t u16;
  uint32_t u32;

  // write 512 bytes to 8-bit register 0x01
  bytesProcessed = twiSlave.writeBuf_reg8(0x01, txBuffer, 512);
  Serial.print("TX result: ");
  Serial.print(bytesProcessed);
  Serial.println(" bytes written out of 512 attempted");

  // read 512 bytes from 8-bit register 0x02
  bytesProcessed = twiSlave.readBuf_reg8(0x02, rxBuffer, 512);
  Serial.print("RX result: ");
  Serial.print(bytesProcessed);
  Serial.println(" bytes read out of 512 requested");
  
  // read 8-bit value (byte) from 8-bit register address 0x03
  twiSlave.read8_reg8(0x03, &u8);
  Serial.print("RX byte: 0x");
  Serial.println(u8, HEX);
  
  // read little-endian 16-bit value (uint16) from 8-bit register address 0x04
  twiSlave.read16le_reg8(0x04, &u16);
  Serial.print("RX 16-bit little-endian value: 0x");
  Serial.println(u16, HEX);
  
  // read big-endian 16-bit value (uint16) from same 8-bit register address 0x04
  twiSlave.read16be_reg8(0x04, &u16);
  Serial.print("RX 16-bit big-endian value: 0x");
  Serial.println(u16, HEX);

  // read little-endian 32-bit value (uint32) from 8-bit register address 0x05
  twiSlave.read32le_reg8(0x05, &u32);
  Serial.print("RX 16-bit little-endian value: 0x");
  Serial.println(u32, HEX);
  
  // read big-endian 32-bit value (uint32) from same 8-bit register address 0x05
  twiSlave.read32be_reg8(0x05, &u32);
  Serial.print("RX 16-bit big-endian value: 0x");
  Serial.println(u32, HEX);
  
  // NOTE: Most I2C peripherals that I have encountered in the wild use 8-bit
  // register addresses. However, 16-bit register addresses do exist, and you
  // can easily use those with Perilib as well. Multi-byte register addresses
  // must be identified (like data values) as little-endian or big-endian. For
  // example:
  //twiSlave.read32le_reg16le(0x1234, &u32);

  // wait between iterations
  delay(5000);
}

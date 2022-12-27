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

uint8_t rxBuffer[32];
PerilibTLVStreamProtocol protocol;
PerilibStreamPacket rxPacket(rxBuffer, sizeof(rxBuffer));
PerilibStreamParserGenerator parser(&protocol, &rxPacket, NULL);

const uint8_t tlvPacket1[] = { 0x01, 0x05, 'H', 'e', 'l', 'l', 'o' };
const uint8_t tlvPacket2[] = { 0x02, 0x05, 0x77, 0x6F, 0x72, 0x6C, 0x64 };
const uint8_t tlvPacket3[] = { 0x03, 0x03, 0x54, 0x4C, 0x56 };
const uint8_t tlvPacket4[] = { 0x04, 0x04, 0x64, 0x65, 0x6D, 0x6F };

int8_t onRxPacket(PerilibStreamPacket *packet)
{
  // dump raw packet data in hexadecimal notation
  Serial.print("RXP: [ ");
  uint16_t i;
  for (i = 0; i < packet->bufferLength; i++)
  {
    if (packet->buffer[i] < 16) Serial.write('0');
    Serial.print(packet->buffer[i], HEX);
    Serial.write(' ');
  }
  Serial.println("]");

  // allow further processing (non-zero to prevent)
  return 0;
}

void setup() {
  // assign callbacks
  parser.onRxPacket = onRxPacket;

  // wait for enumeration with USB-based serial interface (optional)
  //while (!Serial);

  // initialize host serial interface for monitoring
  Serial.begin(9600);
}

void loop() {
  // parse example packets
  parser.parse(tlvPacket1, sizeof(tlvPacket1));
  parser.parse(tlvPacket2, sizeof(tlvPacket2));
  parser.parse(tlvPacket3, sizeof(tlvPacket3));
  parser.parse(tlvPacket4, sizeof(tlvPacket4));

  // wait between iterations
  delay(5000);
}

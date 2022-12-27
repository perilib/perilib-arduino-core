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
PerilibTextStreamProtocol protocol;
PerilibStreamPacket rxPacket(rxBuffer, sizeof(rxBuffer));
PerilibStreamParserGenerator parser(&protocol, &rxPacket, NULL);
PerilibUartStream_ArduinoStream stream(&Serial, &parser);

int8_t onRxPacket(PerilibStreamPacket *packet)
{
  // dump raw packet data
  Serial.print("RXP: [");
  Serial.write(packet->buffer, packet->bufferLength);
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

  // display instructions
  Serial.println("Type commands, end with LF ('\\n', 0x0A)");
}

void loop() {
  // process incoming stream data
  stream.process();
}

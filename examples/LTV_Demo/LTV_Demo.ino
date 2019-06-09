#include <Perilib.h>

uint8_t rxBuffer[32];
Perilib::LTVStreamProtocol protocol;
Perilib::StreamPacket rxPacket;
Perilib::StreamParserGenerator parser(&protocol, &rxPacket, rxBuffer, sizeof(rxBuffer), NULL, NULL, 0);

const uint8_t ltvPacket1[] = { 0x06, 0x01, 'H', 'e', 'l', 'l', 'o' };
const uint8_t ltvPacket2[] = { 0x06, 0x02, 0x77, 0x6F, 0x72, 0x6C, 0x64 };
const uint8_t ltvPacket3[] = { 0x04, 0x03, 0x4C, 0x54, 0x56 };
const uint8_t ltvPacket4[] = { 0x05, 0x04, 0x64, 0x65, 0x6D, 0x6F };

void setup() {
  // initialize host serial interface for monitoring
  Serial.begin(9600);
}

void loop() {
  // parse example packets
  parser.parse(ltvPacket1, sizeof(ltvPacket1));
  parser.parse(ltvPacket2, sizeof(ltvPacket2));
  parser.parse(ltvPacket3, sizeof(ltvPacket3));
  parser.parse(ltvPacket4, sizeof(ltvPacket4));

  // wait between iterations
  delay(5000);
}

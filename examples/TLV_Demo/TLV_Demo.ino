#include <Perilib.h>

uint8_t rxBuffer[32];
Perilib::TLVStreamProtocol protocol;
Perilib::StreamParserGenerator parser(&protocol, rxBuffer, sizeof(rxBuffer));

const uint8_t tlvPacket1[] = { 0x01, 0x05, 'H', 'e', 'l', 'l', 'o' };
const uint8_t tlvPacket2[] = { 0x02, 0x05, 0x77, 0x6F, 0x72, 0x6C, 0x64 };
const uint8_t tlvPacket3[] = { 0x03, 0x03, 0x54, 0x4C, 0x56 };
const uint8_t tlvPacket4[] = { 0x04, 0x04, 0x64, 0x65, 0x6D, 0x6F };

void setup() {
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

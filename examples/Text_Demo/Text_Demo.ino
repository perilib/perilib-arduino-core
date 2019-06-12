#include <Perilib.h>

uint8_t rxBuffer[32];
Perilib::TextStreamProtocol protocol;
Perilib::StreamPacket rxPacket(rxBuffer, sizeof(rxBuffer));
Perilib::StreamParserGenerator parser(&protocol, &rxPacket, NULL);

const uint8_t textPacket1[] = "TEST COMMAND 1\r\n";
const uint8_t textPacket2[] = "TEST ERR" "\x08\x08\x08" "COMMAND 2\r\n";
const uint8_t textPacket3[] = "TEST 3\n";

int8_t onRxPacket(Perilib::StreamPacket *packet)
{
  // dump raw packet data
  Serial.print("RXP: [");
  Serial.write(packet->buffer, packet->bufferPos);
  Serial.println("]");
  
  // allow further processing (non-zero to prevent)
  return 0;
}

void setup() {
  // assign callbacks
  parser.onRxPacket = onRxPacket;
  
  // initialize host serial interface for monitoring
  Serial.begin(9600);
}

void loop() {
  // parse example packets
  parser.parse(textPacket1, sizeof(textPacket1) - 1);
  parser.parse(textPacket2, sizeof(textPacket2) - 1);
  parser.parse(textPacket3, sizeof(textPacket3) - 1);

  // wait between iterations
  delay(5000);
}

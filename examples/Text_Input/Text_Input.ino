#include <Perilib.h>

uint8_t rxBuffer[32];
Perilib::TextStreamProtocol protocol;
Perilib::StreamPacket rxPacket(rxBuffer, sizeof(rxBuffer));
Perilib::StreamParserGenerator parser(&protocol, &rxPacket, NULL);
Perilib::UartStream stream(&Serial, &parser);

int8_t onRxPacket(Perilib::StreamPacket *packet)
{
    Serial.print("RXP: [");
    Serial.println("]");
}

void setup() {
  // assign callbacks
  parser.onRxPacket = onRxPacket;
  
  // wait until host opens serial port
  while (!Serial);
  
  // initialize host serial interface for monitoring
  Serial.begin(9600);
  
  // display instructions
  Serial.println("Type commands, end with LF ('\\n', 0x0A)");
}

void loop() {
  // process incoming stream data
  stream.process();
}

#include <Perilib.h>

Perilib::TextStreamProtocol protocol;
Perilib::StreamParserGenerator parser(&protocol);
Perilib::UartStream stream(&Serial, &parser);

void setup() {
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

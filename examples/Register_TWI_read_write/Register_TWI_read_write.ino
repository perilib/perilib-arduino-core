#include <Perilib.h>

uint8_t rxBuffer[512];
uint8_t txBuffer[512];
Perilib::TwiRegisterInterface_ArduinoWire twi(0x68, &Wire);

void setup() {
  // initialize host serial interface for monitoring
  Serial.begin(9600);
  
  // initialize Wire interface
  Wire.begin();

  // initialize TX buffer with recognizable pattern
  int i;
  for (i = 0; i < 512; i++) txBuffer[i] = i & 0xFF;
}

void loop() {
  // write 512 bytes to register 0x6F
  int txResult = twi.writeBytes(0x6F, txBuffer, 512);
  Serial.print("TX result: ");
  Serial.print(txResult);
  Serial.println(" bytes written out of 512 attempted");

  // read 512 bytes from register 0x6F
  int rxResult = twi.readBytes(0x6F, rxBuffer, 512);
  Serial.print("RX result: ");
  Serial.print(rxResult);
  Serial.println(" bytes read out of 512 requested");

  // wait between iterations
  delay(2000);
}

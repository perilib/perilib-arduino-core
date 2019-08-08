/**
 * Perilib Peripheral Device Library
 * Copyright (c) 2019 Jeff Rowberg
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
 
#include "hal/TwiRegisterInterface_ArduinoWire.h"

#if defined(BUFFER_LENGTH)
    // Arduino AVR core Wire and many others use this
    #define PERILIB_WIRE_BUFFER_LENGTH BUFFER_LENGTH
#elif defined(I2C_BUFFER_LENGTH)
    // Arduino ESP32 core Wire uses this
    #define PERILIB_WIRE_BUFFER_LENGTH I2C_BUFFER_LENGTH
#elif defined(ARDUINO_ARCH_SAMD)
    // Arduino SAMD core Wire uses a 256-byte templated RingBuffer
    #define PERILIB_WIRE_BUFFER_LENGTH 256
#else
    // should be a safe fallback, though slightly inefficient
    #define PERILIB_WIRE_BUFFER_LENGTH 32
#endif

namespace Perilib
{

uint16_t TwiRegisterInterface_ArduinoWire::readBytes(uint8_t regAddr, uint8_t *data, uint16_t length)
{
    PERILIB_DEBUG_PRINT("TwiRegisterInterface::readBytes(");
    PERILIB_DEBUG_PRINT(regAddr);
    PERILIB_DEBUG_PRINT(", *, ");
    PERILIB_DEBUG_PRINT(length);
    PERILIB_DEBUG_PRINTLN(")");

    uint16_t count = 0;

    // check for defined TwoWire interface
    if (arduinoWirePtr)
    {
        // send register address to device
        arduinoWirePtr->beginTransmission(devAddr);
        arduinoWirePtr->write(regAddr);
        arduinoWirePtr->endTransmission();

        // read requested bytes from device
        arduinoWirePtr->beginTransmission(devAddr);
        arduinoWirePtr->requestFrom((int)devAddr, (int)length);
        for (count = 0; arduinoWirePtr->available(); count++) {
            data[count] = arduinoWirePtr->read();
        }
    }
    
    // return number of bytes read
    return count;
}


uint16_t TwiRegisterInterface_ArduinoWire::writeBytes(uint8_t regAddr, uint8_t *data, uint16_t length)
{
    PERILIB_DEBUG_PRINT("TwiRegisterInterface::writeBytes(");
    PERILIB_DEBUG_PRINT(regAddr);
    PERILIB_DEBUG_PRINT(", *, ");
    PERILIB_DEBUG_PRINT(length);
    PERILIB_DEBUG_PRINTLN(")");
    
    uint16_t count = 0;

    // check for defined TwoWire interface
    if (arduinoWirePtr)
    {
        // send data to device
        arduinoWirePtr->beginTransmission(devAddr);
        arduinoWirePtr->write(regAddr);
        arduinoWirePtr->write(data, length);
    	arduinoWirePtr->endTransmission();
    }
    
    // return number of bytes written
    return count;
}

} // namespace Perilib

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
 
#include "hal/TwiRegisterInterface.h"

namespace Perilib
{

uint16_t TwiRegisterInterface::readBytes(uint8_t regAddr, uint8_t *data, uint16_t length)
{
    PERILIB_DEBUG_PRINT("TwiRegisterInterface::readBytes(");
    PERILIB_DEBUG_PRINT(regAddr);
    PERILIB_DEBUG_PRINT(", *, ");
    PERILIB_DEBUG_PRINT(length);
    PERILIB_DEBUG_PRINTLN(")");

    // check for defined TwoWire interface
    if (arduinoTwiPtr)
    {
        // send register address to device
        arduinoTwiPtr->beginTransmission(devAddr);
        arduinoTwiPtr->write(regAddr);
        arduinoTwiPtr->endTransmission();

        // read requested bytes from device
        arduinoTwiPtr->beginTransmission(devAddr);
        arduinoTwiPtr->requestFrom((int)devAddr, (int)length);
        uint16_t count;
        for (count = 0; arduinoTwiPtr->available(); count++) {
            data[count] = arduinoTwiPtr->read();
        }
    }
    
    // failed if we get here, 0 bytes read
    return 0;
}


uint16_t TwiRegisterInterface::writeBytes(uint8_t regAddr, uint8_t *data, uint16_t length)
{
    PERILIB_DEBUG_PRINT("TwiRegisterInterface::writeBytes(");
    PERILIB_DEBUG_PRINT(regAddr);
    PERILIB_DEBUG_PRINT(", *, ");
    PERILIB_DEBUG_PRINT(length);
    PERILIB_DEBUG_PRINTLN(")");

    // check for defined TwoWire interface
    if (arduinoTwiPtr)
    {
        // send data to device
        arduinoTwiPtr->beginTransmission(devAddr);
        arduinoTwiPtr->write(regAddr);
        arduinoTwiPtr->write(data, length);
    	arduinoTwiPtr->endTransmission();
    }
    
    // failed if we get here, 0 bytes written
    return 0;
}

} // namespace Perilib

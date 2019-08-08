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

#if defined(I2C_BUFFER_LENGTH)
    // Arduino ESP32 core Wire uses this
    #define PERILIB_WIRE_BUFFER_LENGTH I2C_BUFFER_LENGTH
#elif defined(BUFFER_LENGTH)
    // Arduino AVR core Wire and many others use this
    #define PERILIB_WIRE_BUFFER_LENGTH BUFFER_LENGTH
#elif defined(SERIAL_BUFFER_SIZE)
    // Arduino SAMD core Wire uses this
    #define PERILIB_WIRE_BUFFER_LENGTH SERIAL_BUFFER_SIZE
#else
    // should be a safe fallback, though possibly inefficient
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

    // ensure TwoWire interface and data pointers are not NULL, and length > 0
    if (arduinoWirePtr && data && length)
    {
        while (length)
        {
            uint16_t chunkSize = PERILIB_WIRE_BUFFER_LENGTH;
            
            // limit chunk size if necessary
            if (chunkSize > length) chunkSize = length;

            // send register address to device
            arduinoWirePtr->beginTransmission(devAddr);
            arduinoWirePtr->write(regAddr);
            arduinoWirePtr->endTransmission();
    
            // read requested bytes from device
            arduinoWirePtr->beginTransmission(devAddr);
            arduinoWirePtr->requestFrom((int)devAddr, (int)chunkSize);
            for (; arduinoWirePtr->available(); count++) {
                data[count] = arduinoWirePtr->read();
            }
            
            // decrease remaining bytes by what we requested to read
            length -= chunkSize;
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

    // ensure TwoWire interface and data pointers are not NULL, and length > 0
    if (arduinoWirePtr && data && length)
    {
        // send data to device
        uint16_t chunkSize = PERILIB_WIRE_BUFFER_LENGTH - 1; // 1 byte for register address
        bool midstream = false;
        while (length)
        {
            if (midstream)
            {
                // mid-transmission, so send current block but don't stop (next block will use repeated start)
                if (arduinoWirePtr->endTransmission(false) == 0) count += chunkSize;
            }
            
            // limit chunk size if necessary
            if (chunkSize > length) chunkSize = length;
            
            // begin new block
            arduinoWirePtr->beginTransmission(devAddr);
            arduinoWirePtr->write(regAddr);
            arduinoWirePtr->write(data + count, chunkSize);
            
            // decrease remaining bytes by what we tried to send
            length -= chunkSize;
            
            // set midstream flag so if we loop, we'll transmit this chunk first
            midstream = true;
        }
        
        // finish transmission with stop signal
        if (arduinoWirePtr->endTransmission() == 0) count += chunkSize;
    }
    
    // NOTE: The endTransmission() method only reports a basic status byte, so
    // this count value only has resolution of chunk size. Each chunk is either
    // successful (100% counted) or unsuccessful (0% counted), but it is
    // possible under some circumstances that some bytes were ACK'd before a
    // NACK is encountered, in which case the reported count here will be lower
    // than the actual number of bytes transmitted. This is unlikely, however,
    // and in any case a situation where reported count is less than requested
    // count should be treated as a failure.

    // return number of bytes written
    return count;
}

} // namespace Perilib

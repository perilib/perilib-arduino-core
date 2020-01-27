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

uint16_t TwiRegisterInterface_ArduinoWire::read(uint32_t regAddr, int8_t regAddrSize, uint8_t *data, int16_t dataLength)
{
    PERILIB_DEBUG_PRINT("TwiRegisterInterface_ArduinoWire::read(");
    PERILIB_DEBUG_PRINT(regAddr);
    PERILIB_DEBUG_PRINT(", ");
    PERILIB_DEBUG_PRINT(regAddrSize);
    PERILIB_DEBUG_PRINT(", *, ");
    PERILIB_DEBUG_PRINT(dataLength);
    PERILIB_DEBUG_PRINTLN(")");

    uint16_t count = 0;
    bool isDataReversed = dataLength < 0;
    if (isDataReversed) dataLength = -dataLength;
    int16_t completeLength = dataLength;

    // ensure TwoWire interface and data pointers are not NULL, and length > 0
    if (arduinoWirePtr && data && dataLength)
    {
        // prepare register address byte order (99% of the time this is one byte anyway)
        uint8_t regAddrOnWire[4];
        if (regAddrSize) regAddrSize = prepareRegAddr(regAddr, regAddrSize, regAddrOnWire);

        bool midstream = false;
        while (dataLength)
        {
            int16_t chunkSize = PERILIB_WIRE_BUFFER_LENGTH - 1; // 1 byte for slave address
            
            if (regAddrSize != 0 && (!midstream || repeatRegAddr))
            {
                // send register address to device
                arduinoWirePtr->beginTransmission((int)slaveAddr);
                arduinoWirePtr->write(regAddrOnWire, regAddrSize);
                if (arduinoWirePtr->endTransmission(stopAfterAddress) != 0) break;
            }

            // limit chunk size if necessary
            if (chunkSize > dataLength) chunkSize = dataLength;

            // if this will be the last read, use the correct stop setting
            if (chunkSize == dataLength) stopAfterChunk = stopAfterComplete;

            // read requested bytes from device
            arduinoWirePtr->requestFrom((int)slaveAddr, (int)chunkSize, stopAfterChunk);
            for (; arduinoWirePtr->available(); count++)
            {
                // read data from Wire HAL into destination buffer
                if (isDataReversed)
                {
                    // store from end of buffer to beginning
                    data[completeLength - count - 1] = arduinoWirePtr->read();
                }
                else
                {
                    // store from beginning of buffer to end (normal)
                    data[count] = arduinoWirePtr->read();
                }
            }
            
            // decrease remaining bytes by what we requested to read
            dataLength -= chunkSize;

            // set midstream flag so if we loop, we know to test chunk restart flag
            midstream = true;
        }
    }
    
    // return number of bytes read
    return count;
}


uint16_t TwiRegisterInterface_ArduinoWire::write(uint32_t regAddr, int8_t regAddrSize, uint8_t *data, int16_t dataLength)
{
    PERILIB_DEBUG_PRINT("TwiRegisterInterface_ArduinoWire::write(");
    PERILIB_DEBUG_PRINT(regAddr);
    PERILIB_DEBUG_PRINT(", ");
    PERILIB_DEBUG_PRINT(regAddrSize);
    PERILIB_DEBUG_PRINT(", *, ");
    PERILIB_DEBUG_PRINT(dataLength);
    PERILIB_DEBUG_PRINTLN(")");
    
    uint16_t count;
    bool isDataReversed = dataLength < 0;
    if (isDataReversed) dataLength = -dataLength;
    int16_t completeLength = dataLength;

    // ensure TwoWire interface and data pointers are not NULL, and length > 0
    if (arduinoWirePtr && data && dataLength)
    {
        // prepare register address byte order (99% of the time this is one byte anyway)
        uint8_t regAddrOnWire[4];
        if (regAddrSize) regAddrSize = prepareRegAddr(regAddr, regAddrSize, regAddrOnWire);
        
        // send data to device
        int16_t chunkSize = PERILIB_WIRE_BUFFER_LENGTH - 1; // 1 byte for slave address
        bool midstream = false;
        count = 0;
        while (dataLength)
        {
            if (midstream)
            {
                // mid-transmission, so send current block
                // (optionally skip STOP, default disable and depends on device implementation)
                if (arduinoWirePtr->endTransmission(stopAfterChunk) == 0) count += chunkSize;
                else break;
            }
            
            // begin new chunk transmission
            arduinoWirePtr->beginTransmission((int)slaveAddr);
            
            // reset chunk size to maximum
            chunkSize = PERILIB_WIRE_BUFFER_LENGTH - 1; // 1 byte for slave address
            
            // send address if this is the first time, or we're supposed to do it every time
            if (regAddrSize != 0 && (!midstream || repeatRegAddr))
            {
                arduinoWirePtr->write(regAddrOnWire, regAddrSize);
                chunkSize -= regAddrSize;
            }

            // limit chunk size if necessary
            if (chunkSize > dataLength) chunkSize = dataLength;
            
            // write data from source buffer into Wire HAL
            if (isDataReversed)
            {
                // send data chunk from end to beginning
                uint16_t i;
                for (i = 0; i < chunkSize; i++)
                {
                    arduinoWirePtr->write(data[completeLength - count - i - 1]);
                }
            }
            else
            {
                // send data chunk as-is
                arduinoWirePtr->write(data + count, chunkSize);
            }
            
            // decrease remaining bytes by what we tried to send
            dataLength -= chunkSize;
            
            // set midstream flag so if we loop, we'll transmit this chunk first
            midstream = true;
        }
        
        // finish transmission with stop signal (only if we didn't break early)
        if (!dataLength && arduinoWirePtr->endTransmission(stopAfterComplete) == 0) count += chunkSize;
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

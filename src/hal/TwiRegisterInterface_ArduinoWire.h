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
 
#ifndef __PERILIB_HAL_TWIREGISTERINTERFACE_ARDUINOWIRE_H__
#define __PERILIB_HAL_TWIREGISTERINTERFACE_ARDUINOWIRE_H__

#include <Wire.h>

#include "../common.h"
#include "../RegisterInterface.h"
#include "../RegisterDevice.h"

namespace Perilib
{

class TwiRegisterInterface_ArduinoWire : public RegisterInterface
{
public:
    TwiRegisterInterface_ArduinoWire(
        uint16_t slaveAddr=0,
        ::TwoWire *arduinoWirePtr=0,
        RegisterMap *registerMapPtr=0,
        RegisterDevice *devicePtr=0)
            : RegisterInterface(registerMapPtr, (RegisterDevice *)devicePtr),
              slaveAddr(slaveAddr),
              repeatRegAddr(true),
              stopAfterComplete(true),
              stopAfterChunk(true),
              stopAfterAddress(false),
              arduinoWirePtr(arduinoWirePtr) { };
    
    // implementation of pure virtual functions in RegisterInterface   
    virtual uint16_t read(uint32_t regAddr, int8_t regAddrSize, uint8_t *data, int16_t dataLength);
    virtual uint16_t write(uint32_t regAddr, int8_t regAddrSize, uint8_t *data, int16_t dataLength);
    
    // ArduinoWire-specific overloads to configure per-transaction behavior
    virtual uint16_t read(uint32_t regAddr, int8_t regAddrSize, uint8_t *data, int16_t dataLength,
        bool repeatRegAddr,
        bool stopAfterComplete,
        bool stopAfterChunk,
        bool stopAfterAddress) {
            this->repeatRegAddr = repeatRegAddr;
            this->stopAfterComplete = stopAfterComplete;
            this->stopAfterChunk = stopAfterChunk;
            this->stopAfterAddress = stopAfterAddress;
            return read(regAddr, regAddrSize, data, dataLength);
        }
        
    virtual uint16_t write(uint32_t regAddr, int8_t regAddrSize, uint8_t *data, int16_t dataLength,
        bool repeatRegAddr,
        bool stopAfterComplete,
        bool stopAfterChunk) {
            this->repeatRegAddr = repeatRegAddr;
            this->stopAfterComplete = stopAfterComplete;
            this->stopAfterChunk = stopAfterChunk;
            return write(regAddr, regAddrSize, data, dataLength);
        }
    
    uint16_t slaveAddr;
    
    bool repeatRegAddr;
    bool stopAfterComplete;
    bool stopAfterChunk;
    bool stopAfterAddress;
    
    ::TwoWire *arduinoWirePtr;
};

} // namespace Perilib

#endif /* __PERILIB_HAL_TWIREGISTERINTERFACE_ARDUINOWIRE_H__ */

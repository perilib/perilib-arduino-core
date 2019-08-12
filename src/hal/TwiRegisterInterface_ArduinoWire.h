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
        uint8_t devAddr=0,
        ::TwoWire *arduinoWirePtr=0,
        RegisterMap *registerMapPtr=0,
        RegisterDevice *devicePtr=0)
            : RegisterInterface(registerMapPtr, (RegisterDevice *)devicePtr),
              devAddr(devAddr),
              arduinoWirePtr(arduinoWirePtr) { };
       
    virtual uint16_t readBytes(uint8_t regAddr, uint8_t *data, uint16_t length);
    virtual uint16_t writeBytes(uint8_t regAddr, uint8_t *data, uint16_t length, bool repeatedStartChunk=false);
    
    uint8_t devAddr;
    
    ::TwoWire *arduinoWirePtr;
};

} // namespace Perilib

#endif /* __PERILIB_HAL_TWIREGISTERINTERFACE_ARDUINOWIRE_H__ */

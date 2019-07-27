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
 
#ifndef __PERILIB_REGISTERINTERFACE_H__
#define __PERILIB_REGISTERINTERFACE_H__

#include "common.h"
#include "RegisterDevice.h"
#include "RegisterMap.h"

namespace Perilib
{

class RegisterInterface
{
public:
    RegisterInterface(
        RegisterMap *registerMapPtr=0,
        RegisterDevice *devicePtr=0)
            : registerMapPtr(registerMapPtr),
              devicePtr(devicePtr) { };

    virtual uint16_t read8(uint8_t regAddr, uint8_t *value) { return readBytes(regAddr, (uint8_t *)&value, 1); }
    virtual uint16_t read16(uint8_t regAddr, uint16_t *value) { return readBytes(regAddr, (uint8_t *)&value, 2); }
    virtual uint16_t read24(uint8_t regAddr, uint32_t *value) { return readBytes(regAddr, (uint8_t *)&value, 3); }
    virtual uint16_t read32(uint8_t regAddr, uint32_t *value) { return readBytes(regAddr, (uint8_t *)&value, 4); }
    virtual uint16_t write8(uint8_t regAddr, uint8_t value) { return writeBytes(regAddr, (uint8_t *)&value, 1); }
    virtual uint16_t write16(uint8_t regAddr, uint16_t value) { return writeBytes(regAddr, (uint8_t *)&value, 2); }
    virtual uint16_t write24(uint8_t regAddr, uint32_t value) { return writeBytes(regAddr, (uint8_t *)&value, 3); }
    virtual uint16_t write32(uint8_t regAddr, uint32_t value) { return writeBytes(regAddr, (uint8_t *)&value, 4); }

    virtual uint16_t readBytes(uint8_t regAddr, uint8_t *value, uint16_t length);
    virtual uint16_t writeBytes(uint8_t regAddr, uint8_t *value, uint16_t length);

    virtual void process(uint8_t mode=ProcessMode::BOTH, bool force=false);

    RegisterMap *registerMapPtr;
    RegisterDevice *devicePtr;
};

} // namespace Perilib

#endif /* __PERILIB_REGISTERINTERFACE_H__ */
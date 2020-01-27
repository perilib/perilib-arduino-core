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

    uint16_t read8           (                  uint8_t  *value) { return read(0,        0, (uint8_t *)value,  1); }    /// Read 8-bit value directly without register address
    uint16_t read8_reg8      (uint8_t  regAddr, uint8_t  *value) { return read(regAddr,  1, (uint8_t *)value,  1); }    /// Read 8-bit value using 8-bit register address
    uint16_t read8_reg16le   (uint16_t regAddr, uint8_t  *value) { return read(regAddr,  2, (uint8_t *)value,  1); }    /// Read 8-bit value using little-endian 16-bit register address
    uint16_t read8_reg16be   (uint16_t regAddr, uint8_t  *value) { return read(regAddr, -2, (uint8_t *)value,  1); }    /// Read 8-bit value using    big-endian 16-bit register address
    uint16_t read16le        (                  uint16_t *value) { return read(0,        0, (uint8_t *)value,  2); }    /// Read little-endian 16-bit value directly without register address
    uint16_t read16be        (                  uint16_t *value) { return read(0,        0, (uint8_t *)value, -2); }    /// Read    big-endian 16-bit value directly without register address
    uint16_t read16le_reg8   (uint8_t  regAddr, uint16_t *value) { return read(regAddr,  1, (uint8_t *)value,  2); }    /// Read little-endian 16-bit value using 8-bit register address
    uint16_t read16be_reg8   (uint8_t  regAddr, uint16_t *value) { return read(regAddr,  1, (uint8_t *)value, -2); }    /// Read    big-endian 16-bit value using 8-bit register address
    uint16_t read16le_reg16le(uint16_t regAddr, uint16_t *value) { return read(regAddr,  2, (uint8_t *)value,  2); }    /// Read little-endian 16-bit value using little-endian 16-bit register address
    uint16_t read16be_reg16le(uint16_t regAddr, uint16_t *value) { return read(regAddr,  2, (uint8_t *)value, -2); }    /// Read    big-endian 16-bit value using little-endian 16-bit register address
    uint16_t read16le_reg16be(uint16_t regAddr, uint16_t *value) { return read(regAddr, -2, (uint8_t *)value,  2); }    /// Read little-endian 16-bit value using little-endian 16-bit register address
    uint16_t read16be_reg16be(uint16_t regAddr, uint16_t *value) { return read(regAddr, -2, (uint8_t *)value, -2); }    /// Read    big-endian 16-bit value using little-endian 16-bit register address
    uint16_t read32le        (                  uint16_t *value) { return read(0,        0, (uint8_t *)value,  4); }    /// Read little-endian 16-bit value directly without register address
    uint16_t read32be        (                  uint16_t *value) { return read(0,        0, (uint8_t *)value, -4); }    /// Read    big-endian 16-bit value directly without register address
    uint16_t read32le_reg8   (uint8_t  regAddr, uint32_t *value) { return read(regAddr,  1, (uint8_t *)value,  4); }    /// Read little-endian 32-bit value using 8-bit register address
    uint16_t read32be_reg8   (uint8_t  regAddr, uint32_t *value) { return read(regAddr,  1, (uint8_t *)value, -4); }    /// Read    big-endian 32-bit value using 8-bit register address
    uint16_t read32le_reg16le(uint16_t regAddr, uint32_t *value) { return read(regAddr,  2, (uint8_t *)value,  4); }    /// Read little-endian 32-bit value using little-endian 16-bit register address
    uint16_t read32be_reg16le(uint16_t regAddr, uint32_t *value) { return read(regAddr,  2, (uint8_t *)value, -4); }    /// Read    big-endian 32-bit value using little-endian 16-bit register address
    uint16_t read32le_reg16be(uint16_t regAddr, uint32_t *value) { return read(regAddr, -2, (uint8_t *)value,  4); }    /// Read little-endian 32-bit value using little-endian 16-bit register address
    uint16_t read32be_reg16be(uint16_t regAddr, uint32_t *value) { return read(regAddr, -2, (uint8_t *)value, -4); }    /// Read    big-endian 32-bit value using little-endian 16-bit register address
    
    uint16_t readBuf          (                  uint8_t *data, int16_t dataLength) { return read(0,        0, data, dataLength); } /// Read arbitrary buffer directly without register address
    uint16_t readBuf_reg8     (uint8_t  regAddr, uint8_t *data, int16_t dataLength) { return read(regAddr,  1, data, dataLength); } /// Read arbitrary buffer using 8-bit register address
    uint16_t readBuf_reg16le  (uint16_t regAddr, uint8_t *data, int16_t dataLength) { return read(regAddr,  2, data, dataLength); } /// Read arbitrary buffer using little-endian 16-bit register address
    uint16_t readBuf_reg16be  (uint16_t regAddr, uint8_t *data, int16_t dataLength) { return read(regAddr, -2, data, dataLength); } /// Read arbitrary buffer using    big-endian 16-bit register address

    uint16_t write8           (                  uint8_t  value) { return write(0,        0, (uint8_t *)&value,  1); }  /// Write 8-bit value directly without register address
    uint16_t write8_reg8      (uint8_t  regAddr, uint8_t  value) { return write(regAddr,  1, (uint8_t *)&value,  1); }  /// Write 8-bit value using 8-bit register address
    uint16_t write8_reg16le   (uint16_t regAddr, uint8_t  value) { return write(regAddr,  2, (uint8_t *)&value,  1); }  /// Write 8-bit value using little-endian 16-bit register address
    uint16_t write8_reg16be   (uint16_t regAddr, uint8_t  value) { return write(regAddr, -2, (uint8_t *)&value,  1); }  /// Write 8-bit value using    big-endian 16-bit register address
    uint16_t write16le        (                  uint16_t value) { return write(0,        0, (uint8_t *)&value,  2); }  /// Write little-endian 16-bit value directly without register address
    uint16_t write16be        (                  uint16_t value) { return write(0,        0, (uint8_t *)&value, -2); }  /// Write    big-endian 16-bit value directly without register address
    uint16_t write16le_reg8   (uint8_t  regAddr, uint16_t value) { return write(regAddr,  1, (uint8_t *)&value,  2); }  /// Write little-endian 16-bit value using 8-bit register address
    uint16_t write16be_reg8   (uint8_t  regAddr, uint16_t value) { return write(regAddr,  1, (uint8_t *)&value, -2); }  /// Write    big-endian 16-bit value using 8-bit register address
    uint16_t write16le_reg16le(uint16_t regAddr, uint16_t value) { return write(regAddr,  2, (uint8_t *)&value,  2); }  /// Write little-endian 16-bit value using little-endian 16-bit register address
    uint16_t write16be_reg16le(uint16_t regAddr, uint16_t value) { return write(regAddr,  2, (uint8_t *)&value, -2); }  /// Write    big-endian 16-bit value using little-endian 16-bit register address
    uint16_t write16le_reg16be(uint16_t regAddr, uint16_t value) { return write(regAddr, -2, (uint8_t *)&value,  2); }  /// Write little-endian 16-bit value using little-endian 16-bit register address
    uint16_t write16be_reg16be(uint16_t regAddr, uint16_t value) { return write(regAddr, -2, (uint8_t *)&value, -2); }  /// Write    big-endian 16-bit value using little-endian 16-bit register address
    uint16_t write32le        (                  uint16_t value) { return write(0,        0, (uint8_t *)&value,  4); }  /// Write little-endian 16-bit value directly without register address
    uint16_t write32be        (                  uint16_t value) { return write(0,        0, (uint8_t *)&value, -4); }  /// Write    big-endian 16-bit value directly without register address
    uint16_t write32le_reg8   (uint8_t  regAddr, uint32_t value) { return write(regAddr,  1, (uint8_t *)&value,  4); }  /// Write little-endian 32-bit value using 8-bit register address
    uint16_t write32be_reg8   (uint8_t  regAddr, uint32_t value) { return write(regAddr,  1, (uint8_t *)&value, -4); }  /// Write    big-endian 32-bit value using 8-bit register address
    uint16_t write32le_reg16le(uint16_t regAddr, uint32_t value) { return write(regAddr,  2, (uint8_t *)&value,  4); }  /// Write little-endian 32-bit value using little-endian 16-bit register address
    uint16_t write32be_reg16le(uint16_t regAddr, uint32_t value) { return write(regAddr,  2, (uint8_t *)&value, -4); }  /// Write    big-endian 32-bit value using little-endian 16-bit register address
    uint16_t write32le_reg16be(uint16_t regAddr, uint32_t value) { return write(regAddr, -2, (uint8_t *)&value,  4); }  /// Write little-endian 32-bit value using little-endian 16-bit register address
    uint16_t write32be_reg16be(uint16_t regAddr, uint32_t value) { return write(regAddr, -2, (uint8_t *)&value, -4); }  /// Write    big-endian 32-bit value using little-endian 16-bit register address

    uint16_t writeBuf         (                  uint8_t *data, int16_t dataLength) { return write(0,        0, data, dataLength); } /// Write arbitrary buffer directly without register address
    uint16_t writeBuf_reg8    (uint8_t  regAddr, uint8_t *data, int16_t dataLength) { return write(regAddr,  1, data, dataLength); } /// Write arbitrary buffer using 8-bit register address
    uint16_t writeBuf_reg16le (uint16_t regAddr, uint8_t *data, int16_t dataLength) { return write(regAddr,  2, data, dataLength); } /// Write arbitrary buffer using little-endian 16-bit register address
    uint16_t writeBuf_reg16be (uint16_t regAddr, uint8_t *data, int16_t dataLength) { return write(regAddr, -2, data, dataLength); } /// Write arbitrary buffer using    big-endian 16-bit register address

    // fundamental read/write operations are pure virtual here since they must be implemented by a hardware-specific layer
    virtual uint16_t read(uint32_t  regAddr, int8_t regAddrSize, uint8_t *data, int16_t dataLength) = 0;
    virtual uint16_t write(uint32_t regAddr, int8_t regAddrSize, uint8_t *data, int16_t dataLength) = 0;

    // as a host/master-driven interface, continuous processing may not be required
    virtual void process(uint8_t mode=ProcessMode::BOTH, bool force=false) { (void)mode; (void)force; }

    RegisterMap *registerMapPtr;
    RegisterDevice *devicePtr;

protected:
    int16_t prepareRegAddr(uint32_t regAddr, int8_t regAddrSize, uint8_t *regAddrOnWire);
};

} // namespace Perilib

#endif /* __PERILIB_REGISTERINTERFACE_H__ */

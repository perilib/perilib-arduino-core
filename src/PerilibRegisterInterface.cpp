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

#include "PerilibRegisterInterface.h"

int16_t PerilibRegisterInterface::prepareRegAddr(uint32_t regAddr, int8_t regAddrSize, uint8_t *regAddrOnWire)
{
    // don't do anything if we have no register address to work with
    if (!regAddrSize) return 0;

    bool isRegAddrBigEndian = (regAddrSize < 0); // check for big-endian
    if (isRegAddrBigEndian) regAddrSize = -regAddrSize; // use absolute value
    if (regAddrSize > 4) regAddrSize = 4; // limit to 32 bits
    if (isRegAddrBigEndian)
    {
        // copy bytes backwards into buffer
        uint8_t *src = (uint8_t *)&regAddr + regAddrSize - 1;
        uint8_t i;
        for (i = 0; i < regAddrSize; i++) *regAddrOnWire++ = *src--;
    }
    else
    {
        // copy bytes directly
        memcpy(regAddrOnWire, (uint8_t *)&regAddr, regAddrSize);
    }

    return regAddrSize;
}

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
 
#include "RegisterInterface.h"

namespace Perilib
{

void RegisterInterface::process(uint8_t mode, bool force)
{
    // suppress unused parameter warnings
    (void)mode;
    (void)force;
}

uint16_t RegisterInterface::readBytes(uint8_t regAddr, uint8_t *data, uint16_t length)
{
    // suppress unused parameter warnings
    (void)regAddr;
    (void)data;
    (void)length;

    PERILIB_DEBUG_PRINT("RegisterInterface::readBytes(");
    PERILIB_DEBUG_PRINT(regAddr);
    PERILIB_DEBUG_PRINT(", *, ");
    PERILIB_DEBUG_PRINT(length);
    PERILIB_DEBUG_PRINTLN(")");

    // STUB: any protocol requiring outgoing data should override this implementation
    return 0;
}

uint16_t RegisterInterface::writeBytes(uint8_t regAddr, uint8_t *data, uint16_t length)
{
    // suppress unused parameter warnings
    (void)regAddr;
    (void)data;
    (void)length;

    PERILIB_DEBUG_PRINT("RegisterInterface::writeBytes(");
    PERILIB_DEBUG_PRINT(regAddr);
    PERILIB_DEBUG_PRINT(", *, ");
    PERILIB_DEBUG_PRINT(length);
    PERILIB_DEBUG_PRINTLN(")");

    // STUB: any protocol requiring outgoing data should override this implementation
    return 0;
}

} // namespace Perilib

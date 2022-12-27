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

#include "PerilibStream.h"

uint16_t PerilibStream::write(const uint8_t *data, uint16_t length)
{
    // suppress unused parameter warnings
    (void)data;
    (void)length;

    PERILIB_DEBUG_PRINT("Stream::write(*, ");
    PERILIB_DEBUG_PRINT(length);
    PERILIB_DEBUG_PRINTLN(")");

    // STUB: any protocol requiring outgoing data should override this implementation
    return 0;
}

void PerilibStream::process(uint8_t mode, bool force)
{
    if (parserGeneratorPtr && (mode == PerilibProcessMode::SUBS || mode == PerilibProcessMode::BOTH))
    {
        parserGeneratorPtr->process(PerilibProcessMode::BOTH, force);
    }
}

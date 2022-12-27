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

#include "PerilibTLVStreamProtocol.h"

int8_t PerilibTLVStreamProtocol::testPacketComplete(const uint8_t *buffer, uint16_t length, PerilibStreamParserGenerator *parserGenerator, bool isTx)
{
    // suppress unused parameter warnings
    (void)parserGenerator;
    (void)isTx;

    // simple terminal condition for TLV data, where T/L are single bytes
    // [type] [length] [v0, v1, ..., v<length>]
    if (length > 1 && length == (uint16_t)(buffer[1] + 2))
    {
        // existing buffer is expected length
        return PerilibParseStatus::COMPLETE;
    }

    // not finished if we made it here
    return PerilibParseStatus::IN_PROGRESS;
}

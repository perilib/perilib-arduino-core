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

#ifndef __PERILIB_STREAMPACKET_H__
#define __PERILIB_STREAMPACKET_H__

#include "PerilibCommon.h"

class PerilibStreamPacket
{
public:
    PerilibStreamPacket(uint8_t *buffer, uint16_t bufferSize) :
            buffer(buffer),
            bufferSize(bufferSize)
    {
        PERILIB_DEBUG_PRINTLN("PerilibStreamPacket::PerilibStreamPacket()");
    }

    virtual int8_t prepareBufferAfterBuilding()
    {
        PERILIB_DEBUG_PRINTLN("PerilibStreamPacket::prepareBufferAfterBuilding()");
        return PerilibResult::OK;
    }

    PerilibStreamParserGenerator *parserGenerator;
    uint8_t *buffer;
    uint16_t bufferSize;
    uint16_t bufferLength;

    uint16_t index;
    const uint8_t *definition;
};

#endif /* __PERILIB_STREAMPACKET_H__ */

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
 
#include "StreamDevice.h"

namespace Perilib
{

int8_t StreamDevice::sendPacket(uint16_t index, ...)
{
    PERILIB_DEBUG_PRINT("StreamDevice::sendPacket(");
    PERILIB_DEBUG_PRINT(index);
    PERILIB_DEBUG_PRINTLN(", ...)");

    int8_t result = Result::OK;

    if (stream && stream->parserGenerator)
    {
        // generate packet
        va_list argv;
        va_start(argv, index);
        result = stream->parserGenerator->generate(index, argv);
        PERILIB_DEBUG_PRINT("generate() result is ");
        PERILIB_DEBUG_PRINTLN(result);
        va_end(argv);
    
        // send packet if stream exists and generation was successful
        if (result == Result::OK)
        {
            result = stream->write(
                stream->parserGenerator->lastTxPacket->buffer,
                stream->parserGenerator->lastTxPacket->bufferLength);
        }
    }
    
    // finished
    return result;
}

void StreamDevice::process(uint8_t mode, bool force)
{
    if (stream && (mode == ProcessMode::SUBS || mode == ProcessMode::BOTH))
    {
        stream->process(ProcessMode::BOTH, force);
    }
}

} // namespace Perilib

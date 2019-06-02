/**
 * Perilib Peripheral Device Library
 * Copyright (c) Jeff Rowberg
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
 
#include "LTVStreamProtocol.h"

namespace Perilib
{

int8_t LTVStreamProtocol::testPacketComplete(const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator, bool isTx)
{
    // simple terminal condition for LTV data, where L/T are single bytes
    // [length] [type] [v0, v1, ..., v<length-1>]
    if (length == (buffer[0] + 1))
    {
        // existing buffer is expected length
        return ParseStatus::COMPLETE;
    }

    // not finished if we made it here
    return ParseStatus::IN_PROGRESS;
}

int8_t LTVStreamProtocol::getPacketFromBuffer(StreamPacket *packet, uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator, bool isTx)
{
    // ensure packet is not null
    if (!packet) return Result::NULL_POINTER;
    
    // pointer packet buffer at RX buffer
    packet->buffer = buffer;
    
    Serial.print("PACKET: [ ");
    uint16_t i;
    for (i = 0; i < length; i++)
    {
        if (buffer[i] < 16) Serial.write('0');
        Serial.print(buffer[i], HEX);
        Serial.write(' ');
    }
    Serial.println("]");
    return Result::OK;
}

} // namespace Perilib

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
 
#include <limits.h>

#include "StreamProtocol.h"

#if UINT_MAX == 0xFFFF
    #define PERILIB_ARG_PROMOTION_BYTES 2
#elif UINT_MAX == 0xFFFFFFFF
    #define PERILIB_ARG_PROMOTION_BYTES 4
#else
    #error Unable to detect int width on this platform, please investigate
#endif

namespace Perilib
{

int8_t StreamProtocol::testPacketStart(const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator, bool isTx)
{
    // suppress unused parameter warnings
    (void)buffer;
    (void)length;
    (void)parserGenerator;
    (void)isTx;

    PERILIB_DEBUG_PRINT("StreamProtocol::testPacketStart(*, ");
    PERILIB_DEBUG_PRINT(length);
    PERILIB_DEBUG_PRINT(", *, ");
    PERILIB_DEBUG_PRINT(isTx);
    PERILIB_DEBUG_PRINTLN(")");

    return ParseStatus::IN_PROGRESS;
}

int8_t StreamProtocol::testPacketComplete(const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator, bool isTx)
{
    // suppress unused parameter warnings
    (void)parserGenerator;
    (void)isTx;

    PERILIB_DEBUG_PRINT("StreamProtocol::testPacketComplete(*, ");
    PERILIB_DEBUG_PRINT(length);
    PERILIB_DEBUG_PRINT(", *, ");
    PERILIB_DEBUG_PRINT(isTx);
    PERILIB_DEBUG_PRINTLN(")");

    // check for simple byte-based terminal condition
    if (terminalByteCount)
    {
        // check for a byte match
        uint8_t i;
        for (i = 0; i < terminalByteCount; i++)
        {
            if (buffer[length - 1] == terminalBytes[i])
            {
                // matching terminal byte, packet is complete
                return ParseStatus::COMPLETE;
            }
        }
        
        // no match, packet is incomplete
        return ParseStatus::IN_PROGRESS;
    }
    
    // no terminal conditions, assume completion after any byte
    return ParseStatus::COMPLETE;
}

int8_t StreamProtocol::getPacketFromIndexAndArgs(StreamPacket *packet, uint16_t index, va_list argv, StreamParserGenerator *parserGenerator)
{
    // suppress unused parameter warnings
    (void)parserGenerator;

    PERILIB_DEBUG_PRINT("StreamProtocol::getPacketFromIndexAndArgs(*, ");
    PERILIB_DEBUG_PRINT(index);
    PERILIB_DEBUG_PRINTLN("..., *)");

    uint8_t i;
    uint16_t size;
    uint16_t dynamicLength = 0;
    uint32_t value;
    uint8_t *pointer;
    
    // make sure packet exists
    if (!packet) return Result::NULL_POINTER;
    
    uint8_t *payload = packet->buffer;

    // get packet definition
    packet->index = index;
    if (getPacketDefinitionFromIndex(packet->index, &packet->definition) != 0) return Result::INVALID_INDEX;
    
    // get first argument in packet definition based on index
    uint8_t argCount = getArgumentCount(packet->index, packet->definition);
    const uint8_t *argDef = getFirstArgument(packet->index, packet->definition);
    if (!argDef) return Result::NULL_POINTER;
    
    // move payload pointer ahead (if necessary) to payload location
    payload += getPayloadOffset(packet->index, packet->definition);

    // iterate over varargs based on number of packet arguments
    for (i = 0; i < argCount; i++, argDef = getNextArgument(packet->index, argDef))
    {
        PERILIB_DEBUG_PRINT("Arg type is ");
        PERILIB_DEBUG_PRINTLN(argDef[0]);
        size = 0;
        pointer = 0;
        switch (argDef[0])
        {
#if PERILIB_ARG_PROMOTION_BYTES == 4
            case StreamProtocol::UINT32:
            case StreamProtocol::INT32:
                /* 4 bytes, start with 2 and fall through two ++ */
                size = 2;
                // FALL THROUGH
            case StreamProtocol::UINT16:
            case StreamProtocol::INT16:
                /* 2 bytes, start with 1 and fall through one ++ */
                size++;
                // FALL THROUGH
            case StreamProtocol::UINT8:
            case StreamProtocol::INT8:
                /* 1 byte */
                size++;
                /* va_arg type is at least 4 bytes wide due to C default argument promotion on 32-bit systems */
                value = va_arg(argv, uint32_t);
                break;
#elif PERILIB_ARG_PROMOTION_BYTES == 2
            case StreamProtocol::UINT32:
            case StreamProtocol::INT32:
                /* 4 bytes */
                size = 4;
                value = va_arg(argv, uint32_t);
                break;
            case StreamProtocol::UINT16:
            case StreamProtocol::INT16:
                /* 2 bytes, start with 1 and fall through one ++ */
                size = 1;
                // FALL THROUGH
            case StreamProtocol::UINT8:
            case StreamProtocol::INT8:
                /* 1 byte */
                size++;
                /* va_arg type is at least 2 bytes wide due to C default argument promotion on 8-bit/16-bit systems */
                value = va_arg(argv, uint16_t);
                break;
#endif
            case StreamProtocol::MACADDR:
                /* 6 bytes exactly, start with 4 and fall through two ++ */
                size = 4;
                // FALL THROUGH
            case StreamProtocol::LONGUINT8A:
                /* 2 bytes minimum, start with 1 and fall through one ++ */
                size++;
                // FALL THROUGH
            case StreamProtocol::UINT8A:
                /* 1 byte minimum */
                size++;
                pointer = (uint8_t *)va_arg(argv, uint8_t *);
                break;
                
            default:
                /* should never occur, all cases covered */
                break;
        }
        
        /* check for correct type */
        if (pointer != 0)
        {
            /* pointer to uint8a_t, longuint8a_t, or macaddr_t */
            if (size == 1)
            {
                /* uint8a_t, first byte is buffer length */
                size += pointer[0];
                
                /* adjust payload length in header */
                dynamicLength += pointer[0];
            }
            else if (size == 2)
            {
                /* longuint8a_t, first two bytes are buffer length */
                size += pointer[0] + (pointer[1] << 8);
                
                /* adjust payload length in header */
                dynamicLength += pointer[0] + (pointer[1] << 8);
            }
            else if (size == 6)
            {
                /* macaddr_t */
                /* copy 6 bytes directly */
            }
        }
        else
        {
            /* numeric value passed, use it directly */
            pointer = (uint8_t *)&value;
        }

        PERILIB_DEBUG_PRINT("Copying ");
        PERILIB_DEBUG_PRINT(size);
        PERILIB_DEBUG_PRINT(" bytes to buffer offset ");
        PERILIB_DEBUG_PRINTLN(payload - packet->buffer);
        
        memcpy(payload, pointer, size);
        payload += size;
    }
    
    // update packet length to reflect everything added so far
    packet->bufferLength = payload - packet->buffer;

    // success so far, finalize the packet and return
    return packet->prepareBufferAfterBuilding();
}

int8_t StreamProtocol::getPacketFromBuffer(StreamPacket *packet, uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator, bool isTx)
{
    // suppress unused parameter warnings
    (void)packet;
    (void)buffer;
    (void)length;
    (void)parserGenerator;
    (void)isTx;

    PERILIB_DEBUG_PRINT("StreamProtocol::getPacketFromBuffer(*, *, ");
    PERILIB_DEBUG_PRINT(length);
    PERILIB_DEBUG_PRINT(", *, ");
    PERILIB_DEBUG_PRINT(isTx);
    PERILIB_DEBUG_PRINTLN(")");

    // STUB: most protocols should override this implementation
    return 0;
}

int8_t StreamProtocol::getPacketDefinitionFromIndex(uint16_t index, const uint8_t **packetDef)
{
    // suppress unused parameter warnings
    (void)index;
    (void)packetDef;

    PERILIB_DEBUG_PRINT("StreamProtocol::getPacketDefinitionFromIndex(");
    PERILIB_DEBUG_PRINT(index);
    PERILIB_DEBUG_PRINTLN(", *)");

    // STUB: most protocols should override this implementation
    return Result::NOT_IMPLEMENTED;
}

int8_t StreamProtocol::getPacketDefinitionFromBuffer(const uint8_t *buffer, uint16_t length, bool isTx, uint16_t *index, const uint8_t **packetDef)
{
    // suppress unused parameter warnings
    (void)buffer;
    (void)length;
    (void)isTx;
    (void)index;
    (void)packetDef;

    PERILIB_DEBUG_PRINT("StreamProtocol::getPacketDefinitionFromBuffer(*, ");
    PERILIB_DEBUG_PRINT(length);
    PERILIB_DEBUG_PRINT(", ");
    PERILIB_DEBUG_PRINT(isTx);
    PERILIB_DEBUG_PRINTLN(", *, *)");

    // STUB: most protocols should override this implementation
    return Result::NOT_IMPLEMENTED;
}

uint8_t StreamProtocol::getArgumentCount(uint16_t index, const uint8_t *packetDef)
{
    // suppress unused parameter warnings
    (void)index;
    (void)packetDef;

    PERILIB_DEBUG_PRINT("StreamProtocol::getArgumentCount(");
    PERILIB_DEBUG_PRINT(index);
    PERILIB_DEBUG_PRINTLN(", *)");

    // STUB: most protocols should override this implementation
    return 0;
}

const uint8_t *StreamProtocol::getFirstArgument(uint16_t index, const uint8_t *packetDef)
{
    // suppress unused parameter warnings
    (void)index;
    (void)packetDef;

    PERILIB_DEBUG_PRINT("StreamProtocol::getFirstArgument(");
    PERILIB_DEBUG_PRINT(index);
    PERILIB_DEBUG_PRINTLN(", *)");

    // STUB: most protocols should override this implementation
    return 0;
}

const uint8_t *StreamProtocol::getNextArgument(uint16_t index, const uint8_t *argDef)
{
    // suppress unused parameter warnings
    (void)index;

    PERILIB_DEBUG_PRINT("StreamProtocol::getNextArgument(");
    PERILIB_DEBUG_PRINT(index);
    PERILIB_DEBUG_PRINTLN(", *)");

    // assume each argument is a single byte
    return argDef + 1;
}

uint16_t StreamProtocol::getPayloadOffset(uint16_t index, const uint8_t *packetDef)
{
    // suppress unused parameter warnings
    (void)index;
    (void)packetDef;

    PERILIB_DEBUG_PRINT("StreamProtocol::getPayloadOffset(");
    PERILIB_DEBUG_PRINT(index);
    PERILIB_DEBUG_PRINTLN(", *)");

    // no payload offset unless subclass overrides
    return 0;
}

} // namespace Perilib

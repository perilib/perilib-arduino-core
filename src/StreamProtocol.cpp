#include "StreamProtocol.h"

namespace Perilib
{

int8_t StreamProtocol::testPacketStart(const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator, bool isTx)
{
    PERILIB_DEBUG_PRINT("StreamProtocol::testPacketStart(*, ");
    PERILIB_DEBUG_PRINT(length);
    PERILIB_DEBUG_PRINT(", *, ");
    PERILIB_DEBUG_PRINT(isTx);
    PERILIB_DEBUG_PRINTLN(")");

    return ParseStatus::IN_PROGRESS;
}

int8_t StreamProtocol::testPacketComplete(const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator, bool isTx)
{
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
    
    // get first argument in packet definition based on index
    const uint8_t *packetDef;
    if (getPacketDefinition(index, &packetDef) != 0) return Result::INVALID_INDEX;
    uint8_t argCount = getArgumentCount(index, packetDef);
    const uint8_t *argDef = getFirstArgument(index, packetDef);
    if (!argDef) return Result::NULL_POINTER;
    
    // move payload pointer ahead (if necessary) to payload location
    payload += getPayloadOffset(index, packetDef);

    // iterate over varargs based on number of packet arguments
    for (i = 0; i < argCount; i++, argDef = getNextArgument(index, argDef))
    {
        size = 0;
        pointer = 0;
        switch (argDef[0])
        {
#if PERILIB_ARG_PROMOTION_32BIT
            case StreamProtocol::UINT32:
            case StreamProtocol::INT32:
                /* 4 bytes, start with 2 and fall through two ++ */
                size = 2;
            case StreamProtocol::UINT16:
            case StreamProtocol::INT16:
                /* 2 bytes, start with 1 and fall through one ++ */
                size = 1;
            case StreamProtocol::UINT8:
            case StreamProtocol::INT8:
                /* 1 byte */
                size++;
                /* va_arg type is at least 4 bytes wide due to C default argument promotion on 32-bit systems */
                value = va_arg(argv, uint32_t);
                break;
#elif PERILIB_ARG_PROMOTION_16BIT
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
            case StreamProtocol::UINT8:
            case StreamProtocol::INT8:
                /* 1 byte */
                size++;
                /* va_arg type is NEVER 1 byte wide due to C default argument promotion on 8-bit/16-bit systems */
                value = va_arg(argv, uint16_t);
                break;
#endif
            case StreamProtocol::MACADDR:
                /* 6 bytes exactly, start with 4 and fall through two ++ */
                size = 4;
            case StreamProtocol::LONGUINT8A:
                /* 2 bytes minimum, start with 1 and fall through one ++ */
                size++;
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
        memcpy(payload, pointer, size);
        payload += size;
    }
    
    // success so far, finalize the packet and return
    return packet->prepareBufferAfterBuilding();
}

const uint8_t *StreamProtocol::getNextArgument(uint16_t index, const uint8_t *argDef)
{
    PERILIB_DEBUG_PRINT("StreamProtocol::getNextArgument(");
    PERILIB_DEBUG_PRINT(index);
    PERILIB_DEBUG_PRINTLN(", *)");

    // assume each argument is a single byte
    return argDef + 1;
}

uint16_t StreamProtocol::getPayloadOffset(uint16_t index, const uint8_t *packetDef)
{
    PERILIB_DEBUG_PRINT("StreamProtocol::getPayloadOffset(");
    PERILIB_DEBUG_PRINT(index);
    PERILIB_DEBUG_PRINTLN(", *)");

    // no payload offset unless subclass overrides
    return 0;
}

} // namespace Perilib

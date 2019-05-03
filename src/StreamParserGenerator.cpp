#include <stdarg.h> // va_args implementation
#include "StreamParserGenerator.h"

#define PERILIB_ARG_PROMOTION_16BIT 0
#define PERILIB_ARG_PROMOTION_32BIT 1

namespace Perilib
{

void StreamParserGenerator::process(uint8_t mode, bool force)
{
    if (!protocol) return;
    
    // get "now" for reference
    uint32_t now = getTimestampMs();
    
    // check for incoming packet timeout
    if (parserStatus != ParseStatus::IDLE &&
            protocol->incomingPacketTimeoutMs != 0 &&
            (now - incomingPacketT0) > protocol->incomingPacketTimeoutMs)
    {
        // trigger incoming packet timeout handler
        incomingPacketTimedOut();
    }
    
    // check for response packet timeout
    if (responsePending != 0 &&
            protocol->responsePacketTimeoutMs != 0 &&
            (now - responsePacketT0) > protocol->responsePacketTimeoutMs)
    {
        // trigger response packet timeout handler
        responsePacketTimedOut();
    }
}

void StreamParserGenerator::reset()
{
    rxBufferPos = 0;
    parserStatus = ParseStatus::IDLE;
    incomingPacketT0 = 0;
}

int8_t StreamParserGenerator::parse(uint8_t b)
{
    if (!protocol) return -1;
    
    // add byte to buffer (note position is NOT incremented yet, byte may be ignored)
    rxBuffer[rxBufferPos] = b;
    
    if (parserStatus == ParseStatus::IDLE)
    {
        // not already in a packet, so run through start boundary test function
        parserStatus = protocol->testPacketStart(rxBuffer, rxBufferPos + 1, this);
        
        // if we just started and there's a defined timeout, start the timer
        if (parserStatus != ParseStatus::IDLE && protocol->incomingPacketTimeoutMs != 0)
        {
            incomingPacketT0 = getTimestampMs();
        }
    }

    // if we are (or may be) in a packet now, process
    if (parserStatus != ParseStatus::IDLE)
    {
        // check for protocol-defined backspace bytes
        uint8_t i;
        bool backspace = false;
        if (protocol->backspaceByteCount)
        {
            // check for a byte match
            for (i = 0; i < protocol->backspaceByteCount; i++)
            {
                if (b == protocol->backspaceBytes[i])
                {
                    // matching backspace byte, this is a deletion
                    backspace = true;
                }
            }
        }
        
        if (backspace)
        {
            // remove a byte from the buffer, if possible
            if (rxBufferPos > 0)
            {
                rxBufferPos--;
            }
                
            // check for empty buffer
            if (rxBufferPos == 0)
            {
                parserStatus = ParseStatus::IDLE;
            }
        }
        else
        {
            // continue testing start conditions if we haven't fully started yet
            if (parserStatus == ParseStatus::STARTING)
            {
                parserStatus = protocol->testPacketStart(rxBuffer, rxBufferPos + 1, this);
            }
    
            // test for completion conditions if we've fully started
            if (parserStatus == ParseStatus::IN_PROGRESS)
            {
                parserStatus = protocol->testPacketComplete(rxBuffer, rxBufferPos + 1, this);
            }
    
            // increment buffer position to store byte permanently
            // (if the buffer has more space OR this is the end of the packet, since buffer has 1 spare byte)
            if (parserStatus == ParseStatus::COMPLETE || rxBufferPos < rxBufferSize)
            {
                rxBufferPos++;
            }
        }
        
        // process the complete packet if we finished
        if (parserStatus == ParseStatus::COMPLETE)
        {
            // check for protocol-defined trim bytes
            if (protocol->trimByteCount != 0)
            {
                // check for a byte match
                for (i = 0; i < protocol->trimByteCount && rxBufferPos > 0; i++)
                {
                    if (rxBuffer[rxBufferPos - 1] == protocol->trimBytes[i])
                    {
                        // matching trim byte, so remove it
                        rxBufferPos--;
                    }
                }
            }

            // convert the buffer to a packet
            protocol->getPacketFromBuffer(&lastRxPacket, rxBuffer, rxBufferPos, this);
            
            // trigger application-level callback, if defined
            if (onRxPacket)
            {
                onRxPacket(&lastRxPacket);
            }

            // reset the parser
            reset();
        }
    }
    else
    {
        // still idle after parsing a byte, probably malformed/junk data
        incomingPacketT0 = 0;
    }
    
    return parserStatus;
}

int8_t StreamParserGenerator::parse(const uint8_t *data, uint16_t length)
{
    // parse entire buffer
    int8_t result = 0;
    for (; length; length--)
    {
        result = parse(*data++);
    }
    
    return result;
}

int8_t StreamParserGenerator::sendPacket(uint16_t index, ...)
{
    uint8_t i;
    uint16_t size;
    uint16_t dynamicLength = 0;
    uint32_t value;
    uint8_t *pointer;
    uint8_t *payload = 0; // TODO: fix this with device-assigned TX buffer
    va_list argv;

    // ensure protocol is assigned
    if (!protocol) return -1;

    // get first argument in packet definition based on index
    const uint8_t *packetDef;
    if (protocol->getPacketDefinition(index, &packetDef) != 0) return -2;
    uint8_t argCount = protocol->getArgumentCount(packetDef);
    const uint8_t *argDef = protocol->getFirstArgument(packetDef);
    if (!argDef) return -3;
    
    // iterate over varargs based on number of packet arguments
    va_start(argv, index);
    for (i = 0; i < argCount; i++, argDef = protocol->getNextArgument(argDef))
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
    va_end(argv);
    
    return 0;
}

void StreamParserGenerator::incomingPacketTimedOut()
{
    // trigger application-level callback, if defined
    if (onIncomingPacketTimeout)
    {
        onIncomingPacketTimeout(rxBuffer, rxBufferPos, this);
    }
    
    // reset the parser
    reset();
}

void StreamParserGenerator::responsePacketTimedOut()
{
    // trigger application-level callback, if defined
    if (onResponsePacketTimeout)
    {
        onResponsePacketTimeout(responsePending, this);
    }

    // clear pending response info
    responsePending = 0;
    responsePacketT0 = 0;
}

} // namespace Perilib

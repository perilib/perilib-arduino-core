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

int8_t StreamParserGenerator::generate(uint16_t index, va_list argv)
{
    // ensure protocol is assigned
    if (!protocol) return -1;
    
    // create packet
    return protocol->getPacketFromIndexAndArgs(&lastTxPacket, index, argv, this);
}

int8_t StreamParserGenerator::sendPacket(uint16_t index, ...)
{
    va_list argv;
    va_start(argv, index);
    generate(index, argv);
    va_end(argv);

    // TODO: ACTUALLY SEND PACKET
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

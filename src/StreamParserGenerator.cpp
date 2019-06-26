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
 
#include <stdarg.h> // va_args implementation
#include "StreamParserGenerator.h"

namespace Perilib
{

void StreamParserGenerator::process(uint8_t mode, bool force)
{
    // suppress unused parameter warnings
    (void)mode;
    (void)force;

    if (!protocolPtr) return;
    
    // get "now" for reference
    uint32_t now = getTimestampMs();
    
    // check for incoming packet timeout
    if (parserStatus != ParseStatus::IDLE &&
            protocolPtr->incomingPacketTimeoutMs != 0 &&
            (now - incomingPacketT0) > protocolPtr->incomingPacketTimeoutMs)
    {
        // trigger incoming packet timeout handler
        incomingPacketTimedOut();
    }
    
    // check for response packet timeout
    if (responsePending != 0 &&
            protocolPtr->responsePacketTimeoutMs != 0 &&
            (now - responsePacketT0) > protocolPtr->responsePacketTimeoutMs)
    {
        // trigger response packet timeout handler
        responsePacketTimedOut();
    }
}

void StreamParserGenerator::reset()
{
    lastRxPacketPtr->bufferLength = 0;
    parserStatus = ParseStatus::IDLE;
    incomingPacketT0 = 0;
    PERILIB_DEBUG_PRINT("lastRxPacketPtr->bufferLength=");
    PERILIB_DEBUG_PRINTLN(lastRxPacketPtr->bufferLength);
}

int8_t StreamParserGenerator::parse(uint8_t b)
{
    PERILIB_DEBUG_PRINT("StreamParserGenerator::parse(0x");
    PERILIB_DEBUG_PRINTFMT(b, HEX);
    PERILIB_DEBUG_PRINTLN(")");

    if (!protocolPtr) return Result::NULL_POINTER;
    
    // add byte to buffer (note position is NOT incremented yet, byte may be ignored)
    PERILIB_DEBUG_PRINT("Storing byte in buffer index ");
    PERILIB_DEBUG_PRINTLN(lastRxPacketPtr->bufferLength);
    lastRxPacketPtr->buffer[lastRxPacketPtr->bufferLength] = b;
    
    // calculate this once here because we use it a bunch of times
    uint16_t nextBufferLength = lastRxPacketPtr->bufferLength + 1;
    
    if (parserStatus == ParseStatus::IDLE)
    {
        // not already in a packet, so run through start boundary test function
        parserStatus = protocolPtr->testPacketStart(lastRxPacketPtr->buffer, nextBufferLength, this);
        
        // if we just started and there's a defined timeout, start the timer
        if (parserStatus != ParseStatus::IDLE && protocolPtr->incomingPacketTimeoutMs != 0)
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
        if (protocolPtr->backspaceByteCount)
        {
            // check for a byte match
            for (i = 0; i < protocolPtr->backspaceByteCount; i++)
            {
                if (b == protocolPtr->backspaceBytes[i])
                {
                    // matching backspace byte, this is a deletion
                    backspace = true;
                }
            }
        }
        
        if (backspace)
        {
            // remove a byte from the buffer, if possible
            if (lastRxPacketPtr->bufferLength > 0)
            {
                lastRxPacketPtr->bufferLength--;
            }
                
            // check for empty buffer
            if (lastRxPacketPtr->bufferLength == 0)
            {
                parserStatus = ParseStatus::IDLE;
            }
        }
        else
        {
            // continue testing start conditions if we haven't fully started yet
            if (parserStatus == ParseStatus::STARTING)
            {
                parserStatus = protocolPtr->testPacketStart(lastRxPacketPtr->buffer, nextBufferLength, this);
            }
    
            // test for completion conditions if we've fully started
            if (parserStatus == ParseStatus::IN_PROGRESS)
            {
                parserStatus = protocolPtr->testPacketComplete(lastRxPacketPtr->buffer, nextBufferLength, this);
            }
    
            // increment buffer position to store byte permanently
            // (if the buffer has more space OR this is the end of the packet, since buffer has 1 spare byte)
            if (parserStatus == ParseStatus::COMPLETE || nextBufferLength < lastRxPacketPtr->bufferSize)
            {
                lastRxPacketPtr->bufferLength = nextBufferLength;
                PERILIB_DEBUG_PRINT("lastRxPacketPtr->bufferLength=");
                PERILIB_DEBUG_PRINTLN(lastRxPacketPtr->bufferLength);
            }
        }
        
        // process the complete packet if we finished
        if (parserStatus == ParseStatus::COMPLETE)
        {
            // check for protocol-defined trim bytes
            if (protocolPtr->trimByteCount != 0)
            {
                // check for a byte match
                for (i = 0; i < protocolPtr->trimByteCount && lastRxPacketPtr->bufferLength > 0; i++)
                {
                    if (lastRxPacketPtr->buffer[lastRxPacketPtr->bufferLength - 1] == protocolPtr->trimBytes[i])
                    {
                        // matching trim byte, so remove it
                        lastRxPacketPtr->bufferLength--;
                        PERILIB_DEBUG_PRINT("lastRxPacketPtr->bufferLength=");
                        PERILIB_DEBUG_PRINTLN(lastRxPacketPtr->bufferLength);
                    }
                }
            }

            // convert the buffer to a packet
            protocolPtr->getPacketFromBuffer(lastRxPacketPtr, lastRxPacketPtr->buffer, lastRxPacketPtr->bufferLength, this);
            
            // trigger application-level callback, if defined
            if (onRxPacket)
            {
                onRxPacket(lastRxPacketPtr);
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
    
    PERILIB_DEBUG_PRINT("parserStatus=");
    PERILIB_DEBUG_PRINTLN(parserStatus);

    return parserStatus;
}

int8_t StreamParserGenerator::parse(const uint8_t *data, uint16_t length)
{
    // parse entire buffer
    for (; length; length--) parse(*data++);
    return parserStatus;
}

int8_t StreamParserGenerator::generate(uint16_t index, va_list argv)
{
    PERILIB_DEBUG_PRINT("StreamParserGenerator::generate(");
    PERILIB_DEBUG_PRINT(index);
    PERILIB_DEBUG_PRINTLN(", ...)");

    // ensure protocol is assigned
    if (!protocolPtr) return Result::NULL_POINTER;
    
    // create packet
    return protocolPtr->getPacketFromIndexAndArgs(lastTxPacketPtr, index, argv, this);
}

void StreamParserGenerator::incomingPacketTimedOut()
{
    PERILIB_DEBUG_PRINTLN("StreamParserGenerator::incomingPacketTimedOut()");

    // trigger application-level callback, if defined
    if (onIncomingPacketTimeout)
    {
        onIncomingPacketTimeout(lastRxPacketPtr->buffer, lastRxPacketPtr->bufferLength, this);
    }
    
    // reset the parser
    reset();
}

void StreamParserGenerator::responsePacketTimedOut()
{
    PERILIB_DEBUG_PRINTLN("StreamParserGenerator::responsePacketTimedOut()");

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

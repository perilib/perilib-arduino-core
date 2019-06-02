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
 
#include <stdarg.h> // va_args implementation
#include "StreamParserGenerator.h"

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
    PERILIB_DEBUG_PRINT("StreamParserGenerator::parse(0x");
    PERILIB_DEBUG_PRINTFMT(b, HEX);
    PERILIB_DEBUG_PRINTLN(")");

    if (!protocol) return Result::NULL_POINTER;
    
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
            protocol->getPacketFromBuffer(lastRxPacket, rxBuffer, rxBufferPos, this);
            
            // trigger application-level callback, if defined
            if (onRxPacket)
            {
                onRxPacket(lastRxPacket);
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
    if (!protocol) return Result::NULL_POINTER;
    
    // create packet
    return protocol->getPacketFromIndexAndArgs(lastTxPacket, index, argv, this);
}

int8_t StreamParserGenerator::sendPacket(uint16_t index, ...)
{
    PERILIB_DEBUG_PRINT("StreamParserGenerator::sendPacket(");
    PERILIB_DEBUG_PRINT(index);
    PERILIB_DEBUG_PRINTLN(", ...)");

    va_list argv;
    va_start(argv, index);
    generate(index, argv);
    va_end(argv);

    // TODO: ACTUALLY SEND PACKET
    return Result::OK;
}

void StreamParserGenerator::incomingPacketTimedOut()
{
    PERILIB_DEBUG_PRINTLN("StreamParserGenerator::incomingPacketTimedOut()");

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

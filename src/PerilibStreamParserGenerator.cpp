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
#include "PerilibStreamParserGenerator.h"

void PerilibStreamParserGenerator::process(uint8_t mode, bool force)
{
    // suppress unused parameter warnings
    (void)mode;
    (void)force;

    if (!protocolPtr) return;

    // get "now" for reference
    uint32_t now = getTimestampMs();

    // check for incoming packet timeout
    if (parserStatus != PerilibParseStatus::IDLE &&
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

void PerilibStreamParserGenerator::reset()
{
    rxPacketPtr->bufferLength = 0;
    parserStatus = PerilibParseStatus::IDLE;
    incomingPacketT0 = 0;
    PERILIB_DEBUG_PRINT("rxPacketPtr->bufferLength=");
    PERILIB_DEBUG_PRINTLN(rxPacketPtr->bufferLength);
}

int8_t PerilibStreamParserGenerator::parse(uint8_t b)
{
    PERILIB_DEBUG_PRINT("PerilibStreamParserGenerator::parse(0x");
    PERILIB_DEBUG_PRINTFMT(b, HEX);
    PERILIB_DEBUG_PRINTLN(")");

    if (!protocolPtr) return PerilibResult::NULL_POINTER;

    // add byte to buffer (note position is NOT incremented yet, byte may be ignored)
    PERILIB_DEBUG_PRINT("Storing byte in buffer index ");
    PERILIB_DEBUG_PRINTLN(rxPacketPtr->bufferLength);
    rxPacketPtr->buffer[rxPacketPtr->bufferLength] = b;

    // calculate this once here because we use it a bunch of times
    uint16_t nextBufferLength = rxPacketPtr->bufferLength + 1;

    if (parserStatus == PerilibParseStatus::IDLE)
    {
        // not already in a packet, so run through start boundary test function
        parserStatus = protocolPtr->testPacketStart(rxPacketPtr->buffer, nextBufferLength, this);

        // if we just started and there's a defined timeout, start the timer
        if (parserStatus != PerilibParseStatus::IDLE && protocolPtr->incomingPacketTimeoutMs != 0)
        {
            incomingPacketT0 = getTimestampMs();
        }
    }

    // if we are (or may be) in a packet now, process
    if (parserStatus != PerilibParseStatus::IDLE)
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
            if (rxPacketPtr->bufferLength > 0)
            {
                rxPacketPtr->bufferLength--;
            }

            // check for empty buffer
            if (rxPacketPtr->bufferLength == 0)
            {
                parserStatus = PerilibParseStatus::IDLE;
            }
        }
        else
        {
            // continue testing start conditions if we haven't fully started yet
            if (parserStatus == PerilibParseStatus::STARTING)
            {
                parserStatus = protocolPtr->testPacketStart(rxPacketPtr->buffer, nextBufferLength, this);
            }

            // test for completion conditions if we've fully started
            if (parserStatus == PerilibParseStatus::IN_PROGRESS)
            {
                parserStatus = protocolPtr->testPacketComplete(rxPacketPtr->buffer, nextBufferLength, this);
            }

            // increment buffer position to store byte permanently
            // (if the buffer has more space OR this is the end of the packet, since buffer has 1 spare byte)
            if (parserStatus == PerilibParseStatus::COMPLETE || nextBufferLength < rxPacketPtr->bufferSize)
            {
                rxPacketPtr->bufferLength = nextBufferLength;
                PERILIB_DEBUG_PRINT("rxPacketPtr->bufferLength=");
                PERILIB_DEBUG_PRINTLN(rxPacketPtr->bufferLength);
            }
        }

        // process the complete packet if we finished
        if (parserStatus == PerilibParseStatus::COMPLETE)
        {
            // check for protocol-defined trim bytes
            if (protocolPtr->trimByteCount != 0)
            {
                // check for a byte match
                for (i = 0; i < protocolPtr->trimByteCount && rxPacketPtr->bufferLength > 0; i++)
                {
                    if (rxPacketPtr->buffer[rxPacketPtr->bufferLength - 1] == protocolPtr->trimBytes[i])
                    {
                        // matching trim byte, so remove it
                        rxPacketPtr->bufferLength--;
                        PERILIB_DEBUG_PRINT("rxPacketPtr->bufferLength=");
                        PERILIB_DEBUG_PRINTLN(rxPacketPtr->bufferLength);
                    }
                }
            }

            // convert the buffer to a packet
            protocolPtr->getPacketFromBuffer(rxPacketPtr, rxPacketPtr->buffer, rxPacketPtr->bufferLength, this);

            // trigger application-level callback, if defined
            if (onRxPacket)
            {
                onRxPacket(rxPacketPtr);
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

int8_t PerilibStreamParserGenerator::parse(const uint8_t *data, uint16_t length)
{
    // parse entire buffer
    for (; length; length--) parse(*data++);
    return parserStatus;
}

int8_t PerilibStreamParserGenerator::generate(uint16_t index, va_list argv)
{
    PERILIB_DEBUG_PRINT("PerilibStreamParserGenerator::generate(");
    PERILIB_DEBUG_PRINT(index);
    PERILIB_DEBUG_PRINTLN(", ...)");

    // ensure protocol is assigned
    if (!protocolPtr) return PerilibResult::NULL_POINTER;

    // create packet
    return protocolPtr->getPacketFromIndexAndArgs(txPacketPtr, index, argv, this);
}

void PerilibStreamParserGenerator::incomingPacketTimedOut()
{
    PERILIB_DEBUG_PRINTLN("PerilibStreamParserGenerator::incomingPacketTimedOut()");

    // trigger application-level callback, if defined
    if (onIncomingPacketTimeout)
    {
        onIncomingPacketTimeout(rxPacketPtr->buffer, rxPacketPtr->bufferLength, this);
    }

    // reset the parser
    reset();
}

void PerilibStreamParserGenerator::responsePacketTimedOut()
{
    PERILIB_DEBUG_PRINTLN("PerilibStreamParserGenerator::responsePacketTimedOut()");

    // trigger application-level callback, if defined
    if (onResponsePacketTimeout)
    {
        onResponsePacketTimeout(responsePending, this);
    }

    // clear pending response info
    responsePending = 0;
    responsePacketT0 = 0;
}

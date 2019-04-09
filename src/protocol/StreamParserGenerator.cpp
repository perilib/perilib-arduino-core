#include "protocol/StreamParserGenerator.h"

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

void StreamParserGenerator::parse(uint8_t b)
{
    if (!protocol) return;
    
    if (parserStatus == ParseStatus::IDLE)
    {
        // not already in a packet, so run through start boundary test function
        parserStatus = protocol->testPacketStart(NULL, 0, b, this);
        
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
                parserStatus = protocol->testPacketStart(rxBuffer, rxBufferPos, b, this);
            }
    
            // test for completion conditions if we've fully started
            if (parserStatus == ParseStatus::IN_PROGRESS)
            {
                parserStatus = protocol->testPacketComplete(rxBuffer, rxBufferPos, b, this);
            }
    
            // add byte to buffer
            if (rxBufferPos < PERILIB_STREAM_PARSER_RX_BUFFER_SIZE)
            {
                rxBuffer[rxBufferPos++] = b;
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
            protocol->getPacketFromBuffer(NULL, rxBuffer, rxBufferPos, this);
            
            // TODO: send packet to application layer

            // reset the parser
            reset();
        }
    }
    else
    {
        // still idle after parsing a byte, probably malformed/junk data
        incomingPacketT0 = 0;
    }
}

void StreamParserGenerator::parse(const uint8_t *data, uint16_t length)
{
    // parse entire buffer
    for (; length; length--)
    {
        parse(*data++);
    }
}

void StreamParserGenerator::reset()
{
    rxBufferPos = 0;
    parserStatus = ParseStatus::IDLE;
    incomingPacketT0 = 0;
}

void StreamParserGenerator::incomingPacketTimedOut()
{
    // TODO: this
}

void StreamParserGenerator::responsePacketTimedOut()
{
    // TODO: this
}

} // namespace Perilib

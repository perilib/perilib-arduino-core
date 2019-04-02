#include "protocol/StreamParserGenerator.h"

namespace Perilib
{

void StreamParserGenerator::process(uint8_t mode=ProcessMode::BOTH, bool force=false)
{
    // TODO: check for timeouts
}

void StreamParserGenerator::parse(uint8_t b)
{
    if (!protocol) return;
    
    if (parserStatus == ParseStatus::IDLE)
    {
        // not already in a packet, so run through start boundary test function
        parserStatus = protocol->testPacketStart(&b, 1, this);
        
        // if we just started and there's a defined timeout, start the timer
        if (parserStatus != ParseStatus::IDLE && protocol->incomingPacketTimeout != 0)
        {
            incomingPacketT0 = getTimestamp();
        }
    }

    // if we are (or may be) in a packet now, process
    if (parserStatus != ParseStatus::IDLE)
    {
        // add byte to buffer
        if (rxBufferPos < PERILIB_STREAM_PARSER_RX_BUFFER_SIZE)
        {
            rxBuffer[rxBufferPos++] = b;
        }
        
        // continue testing start conditions if we haven't fully started yet
        if (parserStatus == ParseStatus::STARTING)
        {
            parserStatus = protocol->testPacketStart(rxBuffer, rxBufferPos, this);
        }

        // test for completion conditions if we've fully started
        if (parserStatus == ParseStatus::IN_PROGRESS)
        {
            parserStatus = protocol->testPacketComplete(rxBuffer, rxBufferPos, this);
        }

        // process the complete packet if we finished
        if (parserStatus == ParseStatus::COMPLETE)
        {
            // convert the buffer to a packet
            protocol->getPacketFromBuffer(NULL, rxBuffer, rxBufferPos, this);

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

} // namespace Perilib

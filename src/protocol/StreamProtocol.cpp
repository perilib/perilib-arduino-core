#include "protocol/StreamProtocol.h"

namespace Perilib
{

int8_t StreamProtocol::testPacketStart(const uint8_t *buffer, uint16_t length, uint8_t newByte, StreamParserGenerator *parserGenerator, bool isTx)
{
    return ParseStatus::IN_PROGRESS;
}

int8_t StreamProtocol::testPacketComplete(const uint8_t *buffer, uint16_t length, uint8_t newByte, StreamParserGenerator *parserGenerator, bool isTx)
{
    // check for simple byte-based terminal condition
    if (terminalByteCount)
    {
        // check for a byte match
        uint8_t i;
        for (i = 0; i < terminalByteCount; i++)
        {
            if (newByte == terminalBytes[i])
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

int8_t StreamProtocol::getPacketFromBuffer(StreamPacket *packet, const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator, bool isTx)
{
    return 0;
}

int8_t StreamProtocol::getPacketFromNameAndArgs(StreamPacket *packet, const char *packetName, StreamParserGenerator *parserGenerator, ...)
{
    return 0;
}

} // namespace Perilib

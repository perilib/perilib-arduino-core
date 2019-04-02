#include "protocol/StreamProtocol.h"

namespace Perilib
{

int8_t StreamProtocol::testPacketStart(const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator, bool isTx)
{
    return ParseStatus::IN_PROGRESS;
}

int8_t StreamProtocol::testPacketComplete(const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator, bool isTx)
{
    return ParseStatus::COMPLETE;
}

int8_t StreamProtocol::getPacketFromBuffer(StreamPacket *packet, const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator=0, bool isTx=false)
{
    return 0;
}

int8_t StreamProtocol::getPacketFromNameAndArgs(StreamPacket *packet, const char *packetName, StreamParserGenerator *parserGenerator=0, ...)
{
    return 0;
}

} // namespace Perilib

#include "LTVStreamProtocol.h"

namespace Perilib
{

int8_t LTVStreamProtocol::testPacketComplete(const uint8_t *buffer, uint16_t length, uint8_t newByte, StreamParserGenerator *parserGenerator, bool isTx)
{
    // simple terminal condition for LTV data, where L/T are single bytes
    // [length] [type] [v0, v1, ..., v<length>]
    if ((length == 0 && newByte == 0) || (length > 0 && length == (buffer[0])))
    {
        // existing buffer plus new byte is expected length
        return ParseStatus::COMPLETE;
    }

    // not finished if we made it here
    return ParseStatus::IN_PROGRESS;
}

int8_t LTVStreamProtocol::getPacketFromBuffer(StreamPacket *packet, const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator, bool isTx)
{
    Serial.print("PACKET: [ ");
    uint16_t i;
    for (i = 0; i < length; i++)
    {
        if (buffer[i] < 16) Serial.write('0');
        Serial.print(buffer[i], HEX);
        Serial.write(' ');
    }
    Serial.println("]");
    return 0;
}

} // namespace Perilib

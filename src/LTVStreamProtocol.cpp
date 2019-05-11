#include "LTVStreamProtocol.h"

namespace Perilib
{

int8_t LTVStreamProtocol::testPacketComplete(const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator, bool isTx)
{
    // simple terminal condition for LTV data, where L/T are single bytes
    // [length] [type] [v0, v1, ..., v<length-1>]
    if (length == (buffer[0] + 1))
    {
        // existing buffer is expected length
        return ParseStatus::COMPLETE;
    }

    // not finished if we made it here
    return ParseStatus::IN_PROGRESS;
}

int8_t LTVStreamProtocol::getPacketFromBuffer(StreamPacket *packet, uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator, bool isTx)
{
    // ensure packet is not null
    if (!packet) return -1;
    
    // pointer packet buffer at RX buffer
    packet->buffer = buffer;
    
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

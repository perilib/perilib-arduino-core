#include "TLVStreamProtocol.h"

namespace Perilib
{

int8_t TLVStreamProtocol::testPacketComplete(const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator, bool isTx)
{
    // simple terminal condition for TLV data, where T/L are single bytes
    // [type] [length] [v0, v1, ..., v<length>]
    if (length > 1 && length == (buffer[1] + 2))
    {
        // existing buffer is expected length
        return ParseStatus::COMPLETE;
    }

    // not finished if we made it here
    return ParseStatus::IN_PROGRESS;
}

int8_t TLVStreamProtocol::getPacketFromBuffer(StreamPacket *packet, uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator, bool isTx)
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

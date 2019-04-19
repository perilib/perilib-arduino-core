#include "TextStreamProtocol.h"

namespace Perilib
{

const uint8_t TextStreamProtocol::textBackspaceBytes[2] = {0x08, 0x7F};
const uint8_t TextStreamProtocol::textTerminalBytes[1] = {0x0A};
const uint8_t TextStreamProtocol::textTrimBytes[2] = {0x0A, 0x0D};

int8_t TextStreamProtocol::getPacketFromBuffer(StreamPacket *packet, const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator, bool isTx)
{
    Serial.print("PACKET: [ ");
    Serial.write(buffer, length);
    Serial.println(" ]");
    return 0;
}

} // namespace Perilib

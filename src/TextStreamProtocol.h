#ifndef __PERILIB_TEXTSTREAMPROTOCOL_H__
#define __PERILIB_TEXTSTREAMPROTOCOL_H__

#include "common.h"
#include "StreamProtocol.h"

namespace Perilib
{
    
class TextStreamProtocol : public StreamProtocol
{
public:
    TextStreamProtocol()
    {
        backspaceByteCount = sizeof(textBackspaceBytes);
        backspaceBytes = textBackspaceBytes;
        terminalByteCount = sizeof(textTerminalBytes);
        terminalBytes = textTerminalBytes;
        trimByteCount = sizeof(textTrimBytes);
        trimBytes = textTrimBytes;
    }

    virtual int8_t getPacketFromBuffer(StreamPacket *packet, const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator=0, bool isTx=false);

protected:
    static const uint8_t textBackspaceBytes[2];
    static const uint8_t textTerminalBytes[1];
    static const uint8_t textTrimBytes[2];
};

} // namespace Perilib

#endif /* __PERILIB_TEXTSTREAMPROTOCOL_H__ */

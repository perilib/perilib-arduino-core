#ifndef __PERILIB_PROTOCOL_TEXTSTREAMPROTOCOL_H__
#define __PERILIB_PROTOCOL_TEXTSTREAMPROTOCOL_H__

#include "common.h"
#include "protocol/StreamProtocol.h"

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
        trimByteCount = sizeof(textTerminalBytes);
        trimBytes = textTerminalBytes;
    }

protected:
    static const uint8_t textBackspaceBytes[2];
    static const uint8_t textTerminalBytes[2];
};

} // namespace Perilib

#endif /* __PERILIB_PROTOCOL_TEXTSTREAMPROTOCOL_H__ */

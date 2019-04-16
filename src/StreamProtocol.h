#ifndef __PERILIB_STREAMPROTOCOL_H__
#define __PERILIB_STREAMPROTOCOL_H__

#include "common.h"

namespace Perilib
{
    
class StreamProtocol
{
public:
    StreamProtocol() :
            backspaceByteCount(0),
            backspaceBytes(0),
            terminalByteCount(0),
            terminalBytes(0),
            trimByteCount(0),
            trimBytes(0),
            incomingPacketTimeoutMs(0),
            responsePacketTimeoutMs(0) { }
    
    virtual int8_t testPacketStart(const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator=0, bool isTx=false);
    virtual int8_t testPacketComplete(const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator=0, bool isTx=false);
    virtual int8_t getPacketFromBuffer(StreamPacket *packet, const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator=0, bool isTx=false);
    virtual int8_t getPacketFromNameAndArgs(StreamPacket *packet, const char *packetName, StreamParserGenerator *parserGenerator=0, ...);

    uint8_t backspaceByteCount;
    const uint8_t *backspaceBytes;
    
    uint8_t terminalByteCount;
    const uint8_t *terminalBytes;
    
    uint8_t trimByteCount;
    const uint8_t *trimBytes;
    
    uint32_t incomingPacketTimeoutMs;
    uint32_t responsePacketTimeoutMs;
};

} // namespace Perilib

#endif /* __PERILIB_STREAMPROTOCOL_H__ */

#ifndef __PERILIB_PROTOCOL_STREAMPROTOCOL_H__
#define __PERILIB_PROTOCOL_STREAMPROTOCOL_H__

#include "common.h"
#include "protocol/Protocol.h"

namespace Perilib
{
    
class StreamProtocol : public Protocol
{
public:
    StreamProtocol() :
            backspaceByteCount(0),
            backspaceBytes(0),
            terminalByteCount(0),
            terminalBytes(0),
            trimByteCount(0),
            trimBytes(0),
            incomingPacketTimeout(0),
            responsePacketTimeout(0) { }
    
    virtual int8_t testPacketStart(const uint8_t *buffer, uint16_t length, uint8_t newByte, StreamParserGenerator *parserGenerator=0, bool isTx=false);
    virtual int8_t testPacketComplete(const uint8_t *buffer, uint16_t length, uint8_t newByte, StreamParserGenerator *parserGenerator=0, bool isTx=false);
    virtual int8_t getPacketFromBuffer(StreamPacket *packet, const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator=0, bool isTx=false);
    virtual int8_t getPacketFromNameAndArgs(StreamPacket *packet, const char *packetName, StreamParserGenerator *parserGenerator=0, ...);

    uint8_t backspaceByteCount;
    uint8_t *backspaceBytes;
    
    uint8_t terminalByteCount;
    uint8_t *terminalBytes;
    
    uint8_t trimByteCount;
    uint8_t *trimBytes;
    
    uint32_t incomingPacketTimeout;
    uint32_t responsePacketTimeout;
};

} // namespace Perilib

#endif /* __PERILIB_PROTOCOL_STREAMPROTOCOL_H__ */

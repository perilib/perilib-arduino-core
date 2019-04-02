#ifndef __PERILIB_PROTOCOL_STREAMPROTOCOL_H__
#define __PERILIB_PROTOCOL_STREAMPROTOCOL_H__

#include "common.h"
#include "protocol/Protocol.h"

namespace Perilib
{
    
class StreamProtocol : public Protocol
{
public:
    virtual int8_t testPacketStart(const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator=0, bool isTx=false);
    virtual int8_t testPacketComplete(const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator=0, bool isTx=false);
    virtual int8_t getPacketFromBuffer(StreamPacket *packet, const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator=0, bool isTx=false);
    virtual int8_t getPacketFromNameAndArgs(StreamPacket *packet, const char *packetName, StreamParserGenerator *parserGenerator=0, ...);
    
    uint32_t incomingPacketTimeout;
    uint32_t responsePacketTimeout;
};

} // namespace Perilib

#endif /* __PERILIB_PROTOCOL_STREAMPROTOCOL_H__ */

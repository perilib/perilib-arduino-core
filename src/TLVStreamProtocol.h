#ifndef __PERILIB_TLVSTREAMPROTOCOL_H__
#define __PERILIB_TLVSTREAMPROTOCOL_H__

#include "common.h"
#include "StreamProtocol.h"

namespace Perilib
{
    
class TLVStreamProtocol : public StreamProtocol
{
public:
    virtual int8_t testPacketComplete(const uint8_t *buffer, uint16_t length, uint8_t newByte, StreamParserGenerator *parserGenerator=0, bool isTx=false);
    virtual int8_t getPacketFromBuffer(StreamPacket *packet, const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator=0, bool isTx=false);
};

} // namespace Perilib

#endif /* __PERILIB_TLVSTREAMPROTOCOL_H__ */

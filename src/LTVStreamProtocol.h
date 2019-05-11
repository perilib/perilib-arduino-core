#ifndef __PERILIB_LTVSTREAMPROTOCOL_H__
#define __PERILIB_LTVSTREAMPROTOCOL_H__

#include "common.h"
#include "StreamProtocol.h"

namespace Perilib
{
    
class LTVStreamProtocol : public StreamProtocol
{
public:
    virtual int8_t testPacketComplete(const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator=0, bool isTx=false);
    virtual int8_t getPacketFromBuffer(StreamPacket *packet, uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator=0, bool isTx=false);
};

} // namespace Perilib

#endif /* __PERILIB_LTVSTREAMPROTOCOL_H__ */

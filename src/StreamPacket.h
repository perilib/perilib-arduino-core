#ifndef __PERILIB_STREAMPACKET_H__
#define __PERILIB_STREAMPACKET_H__

#include "common.h"

namespace Perilib
{

class StreamPacket
{
public:
    StreamPacket()
    {
        PERILIB_DEBUG_PRINTLN("StreamPacket::StreamPacket()");
    }
    
    virtual int8_t prepareBufferAfterBuilding()
    {
        PERILIB_DEBUG_PRINTLN("StreamPacket::prepareBufferAfterBuilding()");
        return Result::OK;
    }

    StreamParserGenerator *parserGenerator;
    uint8_t *buffer;
    uint16_t bufferLength;

    uint16_t index;
    const uint8_t *definition;
};

} // namespace Perilib

#endif /* __PERILIB_STREAMPACKET_H__ */

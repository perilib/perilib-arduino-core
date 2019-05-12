#ifndef __PERILIB_STREAMPACKET_H__
#define __PERILIB_STREAMPACKET_H__

#include "common.h"

namespace Perilib
{

class StreamPacket
{
public:
    virtual int8_t prepareBufferAfterBuilding() { return Result::OK; }

    StreamParserGenerator *parserGenerator;
    uint8_t *buffer;
};

} // namespace Perilib

#endif /* __PERILIB_STREAMPACKET_H__ */

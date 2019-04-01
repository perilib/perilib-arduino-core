#ifndef __PERILIB_PROTOCOL_STREAMPACKET_H__
#define __PERILIB_PROTOCOL_STREAMPACKET_H__

#include "common.h"
#include "protocol/Packet.h"
#include "protocol/StreamParserGenerator.h"

namespace Perilib
{

class StreamPacket : public Packet
{
public:
    StreamParserGenerator *pargen;
};

} // namespace Perilib

#endif /* __PERILIB_PROTOCOL_STREAMPACKET_H__ */

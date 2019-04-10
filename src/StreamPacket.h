#ifndef __PERILIB_STREAMPACKET_H__
#define __PERILIB_STREAMPACKET_H__

#include "common.h"
#include "StreamParserGenerator.h"

namespace Perilib
{

class StreamPacket
{
public:
    StreamParserGenerator *parserGenerator;
};

} // namespace Perilib

#endif /* __PERILIB_STREAMPACKET_H__ */

#ifndef __PERILIB_PROTOCOL_STREAM_H__
#define __PERILIB_PROTOCOL_STREAM_H__

#include "common.h"
#include "protocol/StreamDevice.h"
#include "protocol/StreamParserGenerator.h"

namespace Perilib
{

class Stream
{
public:
    Stream(
        StreamParserGenerator *parserGenerator=0,
        StreamDevice *device=0)
            : parserGenerator(parserGenerator),
              device(device) { };

    virtual void process(uint8_t mode=ProcessMode::BOTH, bool force=false);

    StreamParserGenerator *parserGenerator;
    StreamDevice *device;
};

} // namespace Perilib

#endif /* __PERILIB_PROTOCOL_STREAM_H__ */

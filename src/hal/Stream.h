#ifndef __PERILIB_HAL_STREAM_H__
#define __PERILIB_HAL_STREAM_H__

#include "common.h"
#include "hal/Device.h"
#include "protocol/StreamParserGenerator.h"

namespace Perilib
{

class Stream
{
public:
    Stream(
        StreamParserGenerator *pargen=0,
        Device *device=0)
            : pargen(pargen),
              device(device) { };

    virtual void process(uint8_t mode=ProcessMode::BOTH, bool force=false);

    StreamParserGenerator *pargen;
    Device *device;
};

} // namespace Perilib

#endif /* __PERILIB_HAL_STREAM_H__ */

#ifndef __PERILIB_HAL_CORE_H__
#define __PERILIB_HAL_CORE_H__

#include "Perilib.h"

namespace Perilib
{

class Device;
class Stream;

class Device
{
public:
    Device(Stream *stream=0) : stream(stream) { };
    virtual void process(uint8_t mode=ProcessMode::PROCESS_BOTH, bool force=false);

    Stream *stream;
};

class Stream
{
public:
    Stream(
        StreamParserGenerator *pargen=0,
        Device *device=0)
            : pargen(pargen),
              device(device) { };

    virtual void process(uint8_t mode=ProcessMode::PROCESS_BOTH, bool force=false);

    Device *device;
    StreamParserGenerator *pargen;
};

} // namespace Perilib

#endif /* __PERILIB_HAL_CORE_H__ */

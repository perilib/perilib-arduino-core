#ifndef __PERILIB_HAL_CORE_H__
#define __PERILIB_HAL_CORE_H__

#include "Perilib.h"

namespace Hal
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
        Perilib::Protocol::StreamParserGenerator *parserGenerator=0,
        Device *device=0)
            : parserGenerator(parserGenerator),
              device(device) { };

    virtual void process(uint8_t mode=Perilib::ProcessMode::PROCESS_BOTH, bool force=false);

    Device *device;
    Perilib::Protocol::StreamParserGenerator *parserGenerator;
};

} // namespace Hal

#endif /* __PERILIB_HAL_CORE_H__ */

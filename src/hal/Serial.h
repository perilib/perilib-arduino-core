#ifndef __PERILIB_HAL_SERIAL_H__
#define __PERILIB_HAL_SERIAL_H__

#include "Perilib.h"

namespace Perilib
{

class SerialDevice;
class SerialStream;

class SerialDevice : public Device
{
public:
    SerialDevice(
        SerialStream *stream=0)
            : Device((Stream *)stream) { };
};

class SerialStream : public Stream
{
public:
    SerialStream(
        ::Stream *arduinoStream=0,
        StreamParserGenerator *parserGenerator=0,
        SerialDevice *device=0)
            : arduinoStream(arduinoStream),
              Stream(parserGenerator, device) { };
       
    virtual void process(uint8_t mode=ProcessMode::PROCESS_BOTH, bool force=false);
              
    ::Stream *arduinoStream;
};

} // namespace Hal

#endif /* __PERILIB_HAL_SERIAL_H__ */

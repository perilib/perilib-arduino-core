#ifndef __PERILIB_HAL_SERIALSTREAM_H__
#define __PERILIB_HAL_SERIALSTREAM_H__

#include "common.h"
#include "hal/Stream.h"
#include "hal/Device.h"
#include "hal/SerialDevice.h"
#include "protocol/StreamParserGenerator.h"

namespace Perilib
{

class SerialStream : public Stream
{
public:
    SerialStream(
        ::Stream *arduinoStream=0,
        StreamParserGenerator *parserGenerator=0,
        SerialDevice *device=0)
            : arduinoStream(arduinoStream),
              Stream(parserGenerator, (Device *)device) { };
       
    virtual void process(uint8_t mode=ProcessMode::PROCESS_BOTH, bool force=false);
              
    ::Stream *arduinoStream;
};

} // namespace Perilib

#endif /* __PERILIB_HAL_SERIALSTREAM_H__ */

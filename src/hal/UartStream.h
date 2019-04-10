#ifndef __PERILIB_HAL_UARTSTREAM_H__
#define __PERILIB_HAL_UARTSTREAM_H__

#include "common.h"
#include "protocol/Stream.h"
#include "protocol/StreamDevice.h"
#include "protocol/StreamParserGenerator.h"

namespace Perilib
{

class UartStream : public Stream
{
public:
    UartStream(
        ::Stream *arduinoUart=0,
        StreamParserGenerator *parserGenerator=0,
        StreamDevice *device=0)
            : Stream(parserGenerator, (StreamDevice *)device),
              arduinoUart(arduinoUart) { };
       
    virtual void process(uint8_t mode=ProcessMode::BOTH, bool force=false);
              
    ::Stream *arduinoUart;
};

} // namespace Perilib

#endif /* __PERILIB_HAL_UARTSTREAM_H__ */

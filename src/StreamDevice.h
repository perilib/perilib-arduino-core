#ifndef __PERILIB_STREAMDEVICE_H__
#define __PERILIB_STREAMDEVICE_H__

#include "common.h"
#include "Device.h"
#include "Stream.h"

namespace Perilib
{

class StreamDevice : public Device
{
public:
    StreamDevice(Stream *stream=0)
            : stream(stream) { };
            
    virtual void process(uint8_t mode=ProcessMode::BOTH, bool force=false);
            
    Stream *stream;
};

} // namespace Perilib

#endif /* __PERILIB_STREAMDEVICE_H__ */

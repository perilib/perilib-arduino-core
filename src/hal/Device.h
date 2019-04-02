#ifndef __PERILIB_HAL_DEVICE_H__
#define __PERILIB_HAL_DEVICE_H__

#include "common.h"
#include "hal/Stream.h"

namespace Perilib
{

class Device
{
public:
    Device(Stream *stream=0) : stream(stream) { };
    virtual void process(uint8_t mode=ProcessMode::BOTH, bool force=false);

    Stream *stream;
};

} // namespace Perilib

#endif /* __PERILIB_HAL_DEVICE_H__ */

#ifndef __PERILIB_HAL_SERIALDEVICE_H__
#define __PERILIB_HAL_SERIALDEVICE_H__

#include "common.h"
#include "hal/Device.h"
#include "hal/SerialStream.h"

namespace Perilib
{

class SerialDevice : public Device
{
public:
    SerialDevice(
        SerialStream *stream=0)
            : Device((Stream *)stream) { };
};

} // namespace Perilib

#endif /* __PERILIB_HAL_SERIALDEVICE_H__ */

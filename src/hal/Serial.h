#ifndef __PERILIB_HAL_SERIAL_H__
#define __PERILIB_HAL_SERIAL_H__

#include <Arduino.h>

#include "hal/Core.h"

class PerilibSerialDevice;
class PerilibSerialStream;

class PerilibSerialDevice : PerilibDevice
{
public:
    PerilibSerialStream &stream;
};

class PerilibSerialStream : PerilibStream
{
public:
    PerilibSerialStream(Stream &stream);
};

#endif /* __PERILIB_HAL_SERIAL_H__ */

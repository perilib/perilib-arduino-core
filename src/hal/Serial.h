#ifndef __PERILIB_HAL_SERIAL_H__
#define __PERILIB_HAL_SERIAL_H__

class PerilibSerialDevice;
class PerilibSerialStream;

class PerilibSerialDevice : PerilibDevice
{
public:
    PerilibSerialDevice(PerilibSerialStream *stream=0) : stream(stream) { };
    
    PerilibSerialStream *stream;
};

class PerilibSerialStream : PerilibStream
{
public:
    PerilibSerialStream(Stream *arduinoStream=0, PerilibSerialDevice *device=0) : arduinoStream(arduinoStream), device(device) { };
    
    Stream *arduinoStream;
    PerilibSerialDevice *device;
};

#endif /* __PERILIB_HAL_SERIAL_H__ */

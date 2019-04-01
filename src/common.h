#ifndef __PERILIB_COMMON_H__
#define __PERILIB_COMMON_H__

#include <Arduino.h>

namespace Perilib
{

enum ProcessMode
{
    PROCESS_SELF = 1,
    PROCESS_SUBS = 2,
    PROCESS_BOTH = 3
};

class Device;
class Stream;
class SerialDevice;
class SerialStream;

class Packet;
class Protocol;
class StreamPacket;
class StreamProtocol;
class StreamParserGenerator;

} // namespace Perilib

#endif /* __PERILIB_COMMON_H__ */

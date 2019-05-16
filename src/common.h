#ifndef __PERILIB_COMMON_H__
#define __PERILIB_COMMON_H__

#include <Arduino.h>

// uncomment for detailed debug output
#define PERILIB_DEBUG

#ifdef PERILIB_DEBUG
#define PERILIB_DEBUG_PRINT(x)          Serial.print(x)
#define PERILIB_DEBUG_PRINTFMT(x, y)    Serial.print(x, y)
#define PERILIB_DEBUG_PRINTLN(x)        Serial.println(x)
#define PERILIB_DEBUG_PRINTLNFMT(x, y)  Serial.println(x, y)
#endif

namespace Perilib
{

enum Result
{
    OK              =  0,
    NULL_POINTER    = -1,
    INVALID_INDEX   = -2,
    UNKNOWN_PACKET  = -3,
};

enum ProcessMode
{
    SELF = 1,
    SUBS = 2,
    BOTH = 3
};

enum ParseStatus
{
    IDLE = 0,
    STARTING = 1,
    IN_PROGRESS = 2,
    COMPLETE = 3
};

class UartStream;

class Device;

class Stream;
class StreamDevice;
class StreamPacket;
class StreamProtocol;
class StreamParserGenerator;

} // namespace Perilib

#endif /* __PERILIB_COMMON_H__ */

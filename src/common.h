/**
 * Perilib Peripheral Device Library
 * Copyright (c) 2019 Jeff Rowberg
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software 
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED *AS IS*, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
 
#ifndef __PERILIB_COMMON_H__
#define __PERILIB_COMMON_H__

#include <Arduino.h>

// uncomment for detailed debug output
//#define PERILIB_DEBUG

#ifdef PERILIB_DEBUG
#define PERILIB_DEBUG_PRINT(x)          Serial.print(x)
#define PERILIB_DEBUG_PRINTFMT(x, y)    Serial.print(x, y)
#define PERILIB_DEBUG_PRINTLN(x)        Serial.println(x)
#define PERILIB_DEBUG_PRINTLNFMT(x, y)  Serial.println(x, y)
#else
#define PERILIB_DEBUG_PRINT(x)
#define PERILIB_DEBUG_PRINTFMT(x, y)
#define PERILIB_DEBUG_PRINTLN(x)
#define PERILIB_DEBUG_PRINTLNFMT(x, y)
#endif

namespace Perilib
{

enum Result
{
    OK              =  0,
    NOT_IMPLEMENTED = -1,
    NULL_POINTER    = -2,
    INVALID_INDEX   = -3,
    UNKNOWN_PACKET  = -4,
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

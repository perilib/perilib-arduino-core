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
 
#ifndef __PERILIB_HAL_UARTSTREAM_H__
#define __PERILIB_HAL_UARTSTREAM_H__

#include "common.h"
#include "Stream.h"
#include "StreamDevice.h"
#include "StreamParserGenerator.h"

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
       
    virtual uint16_t write(const uint8_t *data, uint16_t length);
    virtual void process(uint8_t mode=ProcessMode::BOTH, bool force=false);
              
    ::Stream *arduinoUart;
};

} // namespace Perilib

#endif /* __PERILIB_HAL_UARTSTREAM_H__ */

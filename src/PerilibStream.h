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

#ifndef __PERILIB_STREAM_H__
#define __PERILIB_STREAM_H__

#include "PerilibCommon.h"
#include "PerilibStreamDevice.h"
#include "PerilibStreamParserGenerator.h"

class PerilibStream
{
public:
    PerilibStream(
        PerilibStreamParserGenerator *parserGeneratorPtr=0,
        PerilibStreamDevice *devicePtr=0)
            : parserGeneratorPtr(parserGeneratorPtr),
              devicePtr(devicePtr) { };

    virtual uint16_t write(const uint8_t *data, uint16_t length);
    virtual void process(uint8_t mode=PerilibProcessMode::BOTH, bool force=false);

    PerilibStreamParserGenerator *parserGeneratorPtr;
    PerilibStreamDevice *devicePtr;
};

#endif /* __PERILIB_STREAM_H__ */

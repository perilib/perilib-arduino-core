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

#include "PerilibUartStream_ArduinoStream.h"

uint16_t PerilibUartStream_ArduinoStream::write(const uint8_t *data, uint16_t length)
{
    PERILIB_DEBUG_PRINT("UartStream_ArduinoStream::write(*, ");
    PERILIB_DEBUG_PRINT(length);
    PERILIB_DEBUG_PRINTLN(")");

    // check for defined UART interface
    if (arduinoStreamPtr)
    {
        return arduinoStreamPtr->write(data, length);
    }

    // failed if we get here, 0 bytes sent
    return 0;
}

void PerilibUartStream_ArduinoStream::process(uint8_t mode, bool force)
{
    // check for defined UART stream interface and parser/generator
    if (arduinoStreamPtr && parserGeneratorPtr)
    {
        // check for serial data
        int16_t bytesAvailable = arduinoStreamPtr->available();

        // send all available data (may be none)
        while (bytesAvailable > 0)
        {
            // process next byte from stream
            parserGeneratorPtr->parse(arduinoStreamPtr->read());

            // decrement remaining count
            bytesAvailable--;
        }

        // run processing if needed
        if (mode == PerilibProcessMode::SUBS || mode == PerilibProcessMode::BOTH)
        {
            parserGeneratorPtr->process(PerilibProcessMode::BOTH, force);
        }
    }
}

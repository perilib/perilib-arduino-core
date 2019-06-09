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
 
#ifndef __PERILIB_STREAMPARSERGENERATOR_H__
#define __PERILIB_STREAMPARSERGENERATOR_H__

#include "common.h"
#include "StreamProtocol.h"

namespace Perilib
{
    
class StreamParserGenerator
{
public:
    StreamParserGenerator(
            StreamProtocol *protocol,
            StreamPacket *lastRxPacket, uint8_t *rxBuffer, uint16_t rxBufferSize,
            StreamPacket *lastTxPacket, uint8_t *txBuffer, uint16_t txBufferSize) :
        protocol(protocol),
        lastRxPacket(lastRxPacket),
        rxBuffer(rxBuffer),
        rxBufferSize(rxBufferSize),
        lastTxPacket(lastTxPacket),
        txBuffer(txBuffer),
        txBufferSize(txBufferSize)
        {
            // preset packet buffer pointers to internal par/gen buffers
            if (lastRxPacket)
            {
                lastRxPacket->parserGenerator = this;
                lastRxPacket->buffer = rxBuffer;
            }
            if (lastTxPacket)
            {
                lastTxPacket->parserGenerator = this;
                lastTxPacket->buffer = txBuffer;
            }
        }
    virtual void process(uint8_t mode=ProcessMode::BOTH, bool force=false);
    virtual void reset();
    virtual int8_t parse(uint8_t b);
    virtual int8_t parse(const uint8_t *data, uint16_t length);
    virtual int8_t generate(uint16_t index, va_list ap);
    virtual int8_t sendPacket(uint16_t index, ...);
    virtual uint32_t getTimestampMs() { return millis(); }
    
    int8_t (*onTxPacket)(StreamPacket *packet);
    int8_t (*onRxPacket)(StreamPacket *packet);
    int8_t (*onRxError)(int8_t error, const uint8_t *data, uint16_t length, StreamParserGenerator *parserGenerator);
    int8_t (*onIncomingPacketTimeout)(const uint8_t *data, uint16_t length, StreamParserGenerator *parserGenerator);
    int8_t (*onResponsePacketTimeout)(uint32_t responsePending, StreamParserGenerator *parserGenerator);
    
    StreamProtocol *protocol;
    StreamPacket *lastRxPacket;
    StreamPacket *lastTxPacket;
    
protected:
    virtual void incomingPacketTimedOut();
    virtual void responsePacketTimedOut();
    
    int8_t parserStatus;
    
    uint8_t *rxBuffer;
    uint16_t rxBufferSize;
    uint32_t rxBufferPos;
    
    uint8_t *txBuffer;
    uint16_t txBufferSize;
    
    uint32_t incomingPacketT0;
    uint32_t responsePacketT0;
    uint32_t responsePending;
};

} // namespace Perilib

#endif /* __PERILIB_STREAMPARSERGENERATOR_H__ */

#ifndef __PERILIB_STREAMPARSERGENERATOR_H__
#define __PERILIB_STREAMPARSERGENERATOR_H__

#include "common.h"
#include "StreamProtocol.h"

namespace Perilib
{
    
class StreamParserGenerator
{
public:
    StreamParserGenerator(StreamProtocol *protocol, uint8_t *rxBuffer, uint16_t rxBufferSize, uint8_t *txBuffer, uint16_t txBufferSize) :
        protocol(protocol),
        rxBuffer(rxBuffer),
        rxBufferSize(rxBufferSize),
        txBuffer(txBuffer),
        txBufferSize(txBufferSize) { }
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
    StreamPacket lastRxPacket;
    StreamPacket lastTxPacket;
    
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

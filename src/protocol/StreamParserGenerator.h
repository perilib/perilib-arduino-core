#ifndef __PERILIB_PROTOCOL_STREAMPARSERGENERATOR_H__
#define __PERILIB_PROTOCOL_STREAMPARSERGENERATOR_H__

#include "common.h"
#include "protocol/StreamProtocol.h"

#define PERILIB_STREAM_PARSER_RX_BUFFER_SIZE 32

namespace Perilib
{
    
class StreamParserGenerator
{
public:
    StreamParserGenerator(StreamProtocol *protocol=0) : protocol(protocol) { }
    virtual void process(uint8_t mode=ProcessMode::BOTH, bool force=false);
    virtual void parse(uint8_t b);
    virtual void parse(const uint8_t *data, uint16_t length);
    virtual void reset();
    virtual uint32_t getTimestampMs() { return millis(); }
    
    int8_t (*onIncomingPacketTimeout)(const uint8_t *data, uint16_t length, StreamParserGenerator *parserGenerator);
    int8_t (*onResponsePacketTimeout)(uint32_t responsePending, StreamParserGenerator *parserGenerator);
    
    StreamProtocol *protocol;
    
protected:
    virtual void incomingPacketTimedOut();
    virtual void responsePacketTimedOut();
    
    int8_t parserStatus;
    uint8_t rxBuffer[PERILIB_STREAM_PARSER_RX_BUFFER_SIZE];
    uint32_t rxBufferPos;
    uint32_t incomingPacketT0;
    uint32_t responsePacketT0;
    uint32_t responsePending;
};

} // namespace Perilib

#endif /* __PERILIB_PROTOCOL_STREAMPARSERGENERATOR_H__ */

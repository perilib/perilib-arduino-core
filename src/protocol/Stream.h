#ifndef __PERILIB_PROTOCOL_STREAM_H__
#define __PERILIB_PROTOCOL_STREAM_H__

namespace Protocol
{
    
class StreamProtocol;
class StreamPacket;
class StreamParserGenerator;

class StreamProtocol : public Protocol
{
};

class StreamPacket : public Packet
{
};

class StreamParserGenerator
{
public:
    StreamParserGenerator(StreamProtocol *protocol=0) : protocol(protocol) { }
    virtual void process(uint8_t mode=ProcessMode::PROCESS_BOTH, bool force=false);
    virtual void parse(uint8_t b);
    virtual void parse(const uint8_t *data, uint16_t length);
    
    StreamProtocol *protocol;
};

} // namespace Protocol

#endif /* __PERILIB_PROTOCOL_STREAM_H__ */

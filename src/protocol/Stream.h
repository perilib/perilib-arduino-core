#ifndef __PERILIB_PROTOCOL_STREAM_H__
#define __PERILIB_PROTOCOL_STREAM_H__

namespace Perilib
{
    
class StreamProtocol;
class StreamPacket;
class StreamParserGenerator;

class StreamProtocol : public Protocol
{
};

class StreamPacket : public Packet
{
public:
    StreamParserGenerator *pargen;
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

} // namespace Perilib

#endif /* __PERILIB_PROTOCOL_STREAM_H__ */

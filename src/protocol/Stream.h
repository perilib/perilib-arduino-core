#ifndef __PERILIB_PROTOCOL_STREAM_H__
#define __PERILIB_PROTOCOL_STREAM_H__

class PerilibStreamProtocol;
class PerilibStreamPacket;
class PerilibStreamParserGenerator;

class PerilibStreamProtocol : PerilibProtocol
{
};

class PerilibStreamPacket : PerilibPacket
{
};

class PerilibStreamParserGenerator
{
public:
    PerilibStreamParserGenerator(PerilibStreamProtocol *protocol=0) : protocol(protocol) { }
    
    PerilibStreamProtocol *protocol;
};

#endif /* __PERILIB_PROTOCOL_STREAM_H__ */

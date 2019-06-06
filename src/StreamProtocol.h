/**
 * Perilib Peripheral Device Library
 * Copyright (c) Jeff Rowberg
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
 
#ifndef __PERILIB_STREAMPROTOCOL_H__
#define __PERILIB_STREAMPROTOCOL_H__

#include "common.h"
#include "StreamPacket.h"

namespace Perilib
{
    
class StreamProtocol
{
public:
    typedef enum
    {
        UINT8,          /**< 1 byte, unsigned 8-bit integer */
        INT8,           /**< 1 byte, signed 8-bit integer */
        UINT16,         /**< 2 bytes, unsigned 16-bit integer */
        INT16,          /**< 2 bytes, signed 16-bit integer */
        UINT32,         /**< 4 bytes, unsigned 32-bit integer */
        INT32,          /**< 4 bytes, signed 32-bit integer */
        UINT8A,         /**< 1+ bytes, binary byte array with 1-byte length prefix */
        LONGUINT8A,     /**< 2+ bytes, binary byte array with 2-byte length prefix */
        MACADDR         /**< 6 bytes */
    } argtype_t;

    StreamProtocol() :
            backspaceByteCount(0),
            backspaceBytes(0),
            terminalByteCount(0),
            terminalBytes(0),
            trimByteCount(0),
            trimBytes(0),
            incomingPacketTimeoutMs(0),
            responsePacketTimeoutMs(0) { }
    
    virtual int8_t testPacketStart(const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator=0, bool isTx=false);
    virtual int8_t testPacketComplete(const uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator=0, bool isTx=false);
    virtual int8_t getPacketFromIndexAndArgs(StreamPacket *packet, uint16_t index, va_list argv, StreamParserGenerator *parserGenerator=0);
    /* pure */ virtual int8_t getPacketFromBuffer(StreamPacket *packet, uint8_t *buffer, uint16_t length, StreamParserGenerator *parserGenerator=0, bool isTx=false) = 0;
    /* pure */ virtual int8_t getPacketDefinitionFromIndex(uint16_t index, const uint8_t **packetDef) = 0;
    /* pure */ virtual int8_t getPacketDefinitionFromBuffer(const uint8_t *buffer, uint16_t length, uint16_t *index, const uint8_t **packetDef) = 0;
    /* pure */ virtual uint8_t getArgumentCount(uint16_t index, const uint8_t *packetDef) = 0;
    /* pure */ virtual const uint8_t *getFirstArgument(uint16_t index, const uint8_t *packetDef) = 0;
    virtual const uint8_t *getNextArgument(uint16_t index, const uint8_t *argDef);
    virtual uint16_t getPayloadOffset(uint16_t index, const uint8_t *packetDef);

    uint8_t backspaceByteCount;
    const uint8_t *backspaceBytes;
    
    uint8_t terminalByteCount;
    const uint8_t *terminalBytes;
    
    uint8_t trimByteCount;
    const uint8_t *trimBytes;
    
    uint32_t incomingPacketTimeoutMs;
    uint32_t responsePacketTimeoutMs;
};

} // namespace Perilib

#endif /* __PERILIB_STREAMPROTOCOL_H__ */

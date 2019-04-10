#ifndef __PERILIB_PROTOCOL_DEVICE_H__
#define __PERILIB_PROTOCOL_DEVICE_H__

#include "common.h"

namespace Perilib
{

class Device
{
public:
    virtual void process(uint8_t mode=ProcessMode::BOTH, bool force=false) = 0;
};

} // namespace Perilib

#endif /* __PERILIB_PROTOCOL_DEVICE_H__ */

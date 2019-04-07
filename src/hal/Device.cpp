#include "hal/Device.h"

namespace Perilib
{

void Device::process(uint8_t mode, bool force)
{
    if (stream && (mode == ProcessMode::SUBS || mode == ProcessMode::BOTH))
    {
        stream->process(ProcessMode::BOTH, force);
    }
}

} // namespace Perilib

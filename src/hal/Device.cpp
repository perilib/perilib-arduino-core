#include "hal/Device.h"

namespace Perilib
{

void Device::process(uint8_t mode=ProcessMode::BOTH, bool force=false)
{
    if (stream && (mode == ProcessMode::SUBS || mode == ProcessMode::BOTH))
    {
        stream->process(ProcessMode::BOTH, force);
    }
}

} // namespace Perilib

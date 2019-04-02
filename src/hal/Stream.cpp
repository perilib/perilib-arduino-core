#include "hal/Stream.h"

namespace Perilib
{

void Stream::process(uint8_t mode=ProcessMode::BOTH, bool force=false)
{
    if (pargen && (mode == ProcessMode::SUBS || mode == ProcessMode::BOTH))
    {
        pargen->process(ProcessMode::BOTH, force);
    }
}

} // namespace Perilib

#include "hal/Stream.h"

namespace Perilib
{

void Stream::process(uint8_t mode=ProcessMode::PROCESS_BOTH, bool force=false)
{
    if (pargen && (mode == ProcessMode::PROCESS_SUBS || mode == ProcessMode::PROCESS_BOTH))
    {
        pargen->process(ProcessMode::PROCESS_BOTH, force);
    }
}

} // namespace Perilib

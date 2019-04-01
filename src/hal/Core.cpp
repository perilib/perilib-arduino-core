#include "Perilib.h"

namespace Perilib
{

void Device::process(uint8_t mode=ProcessMode::PROCESS_BOTH, bool force=false)
{
    if (stream && (mode == ProcessMode::PROCESS_SUBS || mode == ProcessMode::PROCESS_BOTH))
    {
        stream->process(ProcessMode::PROCESS_BOTH, force);
    }
}

void Stream::process(uint8_t mode=ProcessMode::PROCESS_BOTH, bool force=false)
{
    if (pargen && (mode == ProcessMode::PROCESS_SUBS || mode == ProcessMode::PROCESS_BOTH))
    {
        pargen->process(ProcessMode::PROCESS_BOTH, force);
    }
}

} // namespace Perilib

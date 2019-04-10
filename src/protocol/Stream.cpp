#include "protocol/Stream.h"

namespace Perilib
{

void Stream::process(uint8_t mode, bool force)
{
    if (parserGenerator && (mode == ProcessMode::SUBS || mode == ProcessMode::BOTH))
    {
        parserGenerator->process(ProcessMode::BOTH, force);
    }
}

} // namespace Perilib

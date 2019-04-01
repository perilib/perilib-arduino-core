#include "Perilib.h"

namespace Perilib
{
    namespace Hal
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
            if (parserGenerator && (mode == ProcessMode::PROCESS_SUBS || mode == ProcessMode::PROCESS_BOTH))
            {
                parserGenerator->process(ProcessMode::PROCESS_BOTH, force);
            }
        }
    }
}

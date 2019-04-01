#include "Perilib.h"

namespace Perilib
{
    namespace Hal
    {
        void SerialStream::process(uint8_t mode=ProcessMode::PROCESS_BOTH, bool force=false)
        {
            // check for defined stream
            if (arduinoStream)
            {
                // check for serial data
                int16_t bytesAvailable = arduinoStream->available();
                
                // check for defined parser/generator
                if (parserGenerator)
                {
                    // send all available data (may be none)
                    while (bytesAvailable > 0)
                    {
                        // process next byte from stream
                        parserGenerator->parse(arduinoStream->read());
                        
                        // decrement remaining count
                        bytesAvailable--;
                    }
                    
                    // run processing if needed
                    if (mode == ProcessMode::PROCESS_SUBS || mode == ProcessMode::PROCESS_BOTH)
                    {
                        parserGenerator->process(ProcessMode::PROCESS_BOTH, force);
                    }
                }
            }
        }
    }
}

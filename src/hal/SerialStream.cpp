#include "hal/SerialStream.h"

namespace Perilib
{

void SerialStream::process(uint8_t mode, bool force)
{
    // check for defined stream
    if (arduinoStream)
    {
        // check for serial data
        int16_t bytesAvailable = arduinoStream->available();
        
        // check for defined parser/generator
        if (pargen)
        {
            // send all available data (may be none)
            while (bytesAvailable > 0)
            {
                // process next byte from stream
                pargen->parse(arduinoStream->read());
                
                // decrement remaining count
                bytesAvailable--;
            }
            
            // run processing if needed
            if (mode == ProcessMode::SUBS || mode == ProcessMode::BOTH)
            {
                pargen->process(ProcessMode::BOTH, force);
            }
        }
    }
}

} // namespace Perilib

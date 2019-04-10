#include "hal/UartStream.h"

namespace Perilib
{

void UartStream::process(uint8_t mode, bool force)
{
    // check for defined UART interface
    if (arduinoUart)
    {
        // check for serial data
        int16_t bytesAvailable = arduinoUart->available();
        
        // check for defined parser/generator
        if (parserGenerator)
        {
            // send all available data (may be none)
            while (bytesAvailable > 0)
            {
                // process next byte from stream
                parserGenerator->parse(arduinoUart->read());
                
                // decrement remaining count
                bytesAvailable--;
            }
            
            // run processing if needed
            if (mode == ProcessMode::SUBS || mode == ProcessMode::BOTH)
            {
                parserGenerator->process(ProcessMode::BOTH, force);
            }
        }
    }
}

} // namespace Perilib

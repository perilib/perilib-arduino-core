#include "Perilib.h"

namespace Perilib
{

void StreamParserGenerator::process(uint8_t mode=ProcessMode::PROCESS_BOTH, bool force=false)
{
    // TODO: check for timeouts
}

void StreamParserGenerator::parse(uint8_t b)
{
    if (!protocol) return;
}

void StreamParserGenerator::parse(const uint8_t *data, uint16_t length)
{
    // parse entire buffer
    for (; length; length--)
    {
        parse(*data++);
    }
}

} // namespace Perilib

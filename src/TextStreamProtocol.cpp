#include "TextStreamProtocol.h"

namespace Perilib
{

const uint8_t TextStreamProtocol::textBackspaceBytes[2] = {0x08, 0x7F};
const uint8_t TextStreamProtocol::textTerminalBytes[1] = {0x0A};
const uint8_t TextStreamProtocol::textTrimBytes[2] = {0x0A, 0x0D};

} // namespace Perilib

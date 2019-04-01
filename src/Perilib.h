#ifndef __PERILIB_H__
#define __PERILIB_H__

#include <Arduino.h>

namespace Perilib
{

enum ProcessMode
{
    PROCESS_SELF = 1,
    PROCESS_SUBS = 2,
    PROCESS_BOTH = 3
};

#include "protocol/Core.h"
#include "protocol/Stream.h"
#include "hal/Core.h"
#include "hal/Serial.h"

} // namespace Perilib

#endif /* __PERILIB_H__ */

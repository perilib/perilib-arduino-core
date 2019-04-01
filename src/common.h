#ifndef __PERILIB_COMMON_H__
#define __PERILIB_COMMON_H__

#include <Arduino.h>

namespace Perilib
{

enum ProcessMode
{
    PROCESS_SELF = 1,
    PROCESS_SUBS = 2,
    PROCESS_BOTH = 3
};

} // namespace Perilib

#endif /* __PERILIB_COMMON_H__ */

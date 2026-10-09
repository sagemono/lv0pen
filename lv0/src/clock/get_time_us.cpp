#include "clock.h"
unsigned long get_time_us(void)
{
    return read_timebase() / 79;
}

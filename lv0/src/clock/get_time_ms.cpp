#include "clock.h"
unsigned long get_time_ms(void)
{
    return read_timebase() / 79000;
}

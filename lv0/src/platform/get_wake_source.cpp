#include "config.h"
#include "syscon.h"

long get_wake_source(void)
{
    struct {
        unsigned char f[4];
        unsigned char f2;
        unsigned int wake_source;
    } w;

    if (syscon_get_wake_info(&w.f[0], &w.f2, &w.f[3], &w.f[2], &w.f[1], &w.wake_source) == 0)
        return w.wake_source;
    return 0;
}

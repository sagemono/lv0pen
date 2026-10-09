#include "clock.h"

#define HDEC(v) __asm__ volatile ("mfspr %0,310" : "=r"(v))

unsigned int timebase_frequency = 79800000;

void delay_hdec_ticks(int ticks)
{
    volatile int hdec;
    int start, end, now;
    HDEC(hdec);
    start = hdec;
    if (ticks <= 0)
        return;
    end = start - ticks;
    if (end < start) {
        do {
            HDEC(hdec);
            now = hdec;
        } while (end < now && now <= start);
    } else {
        do {
            HDEC(hdec);
            now = hdec;
        } while (end < now || now <= start);
    }
}

long delay_ms(int ms)
{
    delay_hdec_ticks(timebase_frequency / 1000 * ms);
}

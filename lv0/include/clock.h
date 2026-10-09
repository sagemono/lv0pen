#ifndef LV0_CLOCK_H
#define LV0_CLOCK_H

#include "lv0.h"

unsigned long read_timebase(void);
unsigned long get_time_us(void);
unsigned long get_time_ms(void);
unsigned int get_be_revision(void);
long get_core_clock_multiplier(void);
long get_reference_clock(void);
void delay_hdec_ticks(int ticks);
long delay_ms(int ms);

#endif

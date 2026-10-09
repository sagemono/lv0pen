#include "lv0.h"
#include "platform.h"

void operator delete(void *p) throw()
{
    hang_forever();
}

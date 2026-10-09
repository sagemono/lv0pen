#include "config.h"
#include "syscon.h"

int read_eeprom_nv0_at19(void *dst)
{
    return nv_storage::read(0, 19, 42, dst, 0x48000);
}

int read_qa_token(void *dst)
{
    if (read_eeprom_qa_token(dst))
        return -1;
    if (read_eeprom_nv0_at19((char *)dst + 80))
        return -1;
    return 0;
}

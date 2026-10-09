#include "config.h"

bool is_qaf_enabled(void)
{
    unsigned char qa_flag[16];
    if (read_eeprom_qa_flag(qa_flag) != 0 || qa_flag[0] == 0xFF)
        return false;
    return true;
}

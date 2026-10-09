#include "sb.h"

sb_device *get_sb_device(void)
{
    static sb_device device;
    return &device;
}

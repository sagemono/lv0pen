#include "sb.h"

sb_device *get_sb_device()
{
    static sb_device device;
    return &device;
}

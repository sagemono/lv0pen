#include "nand_flash.h"

nand_flash_ctrl *get_nand_flash_ctrl()
{
    static nand_flash_ctrl ctrl;
    return &ctrl;
}

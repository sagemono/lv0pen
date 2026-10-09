#include "config.h"
typedef int gbe_macaddr_reader_fn(unsigned long *out);

gbe_macaddr_reader_fn *const gbe_macaddr_reader_table[4] = {
    read_eeprom_gbe_macaddr, read_eeprom_gbe_macaddr,
    read_eeprom_gbe_macaddr, read_eeprom_gbe_macaddr,
};

unsigned long get_gbe_macaddr(unsigned int id)
{
    struct {
        unsigned long macaddr;
        gbe_macaddr_reader_fn *readers[4];
    } buf;
    gbe_macaddr_reader_fn *fn;

    buf.readers[0] = gbe_macaddr_reader_table[0];
    buf.readers[1] = gbe_macaddr_reader_table[1];
    buf.readers[2] = gbe_macaddr_reader_table[2];
    buf.readers[3] = gbe_macaddr_reader_table[3];

    if (id > 3)
        return 0xFFFFFFFFFFFFUL;

    fn = buf.readers[id];
    buf.macaddr = 0;
    if (fn(&buf.macaddr) != 0)
        return 0xFFFFFFFFFFFFUL;

    return buf.macaddr >> 16;
}

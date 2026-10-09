#include "hash_reader.h"

int bytes_differ(const unsigned char *a, const unsigned char *b, int n);

long hash_reader::verify(const unsigned char *d, unsigned int n)
{
    if (!d || n != 20)
        return -1;
    if (bytes_differ(d, m_digest, 20))
        return -1;
    return 0;
}

#include "plain_cipher.h"
#include "util.h"

plain_cipher g_plain_cipher;

void plain_cipher::set_key(const unsigned char *key, const unsigned char *iv)
{
}

int plain_cipher::vslot6(const void *state)
{
    return 0;
}

int plain_cipher::vslot7(void *state)
{
    return 0;
}

int plain_cipher::decrypt(const void *in, int len, void *out)
{
    memcpy(out, in, len);
    return 0;
}

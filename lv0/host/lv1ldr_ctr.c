#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aes.h"

#define VEC_RK 0x108
#define VEC_IV 0x1B8

static unsigned char *read_file(const char *path, size_t *len)
{
    FILE *f = fopen(path, "rb");
    unsigned char *buf;
    long n;

    if (!f || fseek(f, 0, SEEK_END) != 0 || (n = ftell(f)) < 0 || fseek(f, 0, SEEK_SET) != 0) {
        perror(path);
        exit(1);
    }
    buf = malloc(n ? n : 1);
    if (!buf || fread(buf, 1, n, f) != (size_t)n) {
        perror(path);
        exit(1);
    }
    fclose(f);
    *len = n;
    return buf;
}

int main(int argc, char **argv)
{
    unsigned int rk[4 * (AES_ROUNDS + 1)];
    unsigned char ctr[16], *vec, *in, *out;
    size_t vec_len, in_len, size, i;
    char *end;
    FILE *f;
    int j;

    if (argc != 5) {
        fprintf(stderr, "usage: %s VEC IN OUT SIZE\n", argv[0]);
        return 2;
    }
    size = strtoul(argv[4], &end, 0);
    if (*end || size % 16) {
        fprintf(stderr, "%s: SIZE %s is not a multiple of 16\n", argv[0], argv[4]);
        return 2;
    }

    vec = read_file(argv[1], &vec_len);
    if (vec_len < VEC_IV + 16) {
        fprintf(stderr, "%s: %s is too short for the key at 0x%x\n", argv[0], argv[1], VEC_RK);
        return 1;
    }
    for (i = 0; i < 4 * (AES_ROUNDS + 1); i++) {
        const unsigned char *p = vec + VEC_RK + 4 * i;
        rk[i] = (unsigned int)p[0] << 24 | (unsigned int)p[1] << 16 | (unsigned int)p[2] << 8 | p[3];
    }
    memcpy(ctr, vec + VEC_IV, 16);

    in = read_file(argv[2], &in_len);
    if (in_len > size) {
        fprintf(stderr, "%s: %s is %lu bytes, more than %lu\n", argv[0], argv[2],
                (unsigned long)in_len, (unsigned long)size);
        return 1;
    }
    out = calloc(size ? size : 1, 1);
    if (!out)
        return 1;
    memcpy(out, in, in_len);

    for (i = 0; i < size; i += 16) {
        aes_ctr_crypt(rk, ctr, 16, out + i, out + i);
        for (j = 15; j >= 0; j--)
            if (++ctr[j] != 0)
                break;
    }

    f = fopen(argv[3], "wb");
    if (!f || fwrite(out, 1, size, f) != size || fclose(f) != 0) {
        perror(argv[3]);
        return 1;
    }
    return 0;
}

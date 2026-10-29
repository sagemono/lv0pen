#ifndef LDR_SHA1_H
#define LDR_SHA1_H

#include <spu_intrinsics.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    vec_uint4 h[5];
    unsigned long long count;
    unsigned char buf[64] __attribute__((aligned(16)));
    int num;
} sha1_ctx;

int sha1_init(sha1_ctx *ctx);
int sha1_update(sha1_ctx *ctx, const unsigned char *data, int len);
int sha1_final(unsigned char *md, sha1_ctx *ctx);
int sha1_transform(sha1_ctx *ctx, const unsigned char *data, int blocks);

#ifdef __cplusplus
}
#endif

#endif

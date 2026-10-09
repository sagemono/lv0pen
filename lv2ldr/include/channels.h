#ifndef LV2LDR_CHANNELS_H
#define LV2LDR_CHANNELS_H

#include <spu_intrinsics.h>
#include "types.h"

int ch72_put_request(u32 cmd, const unsigned char *payload);
int ch72_get_reply(unsigned char *payload);
int ch72_get_version(u64 *version);
int ch72_compare_version(u64 version);
int ch72_unwrap_iv(vec_uchar16 *out, const vec_uchar16 *in, int len);
int ch72_unwrap_key(vec_uchar16 *out, const vec_uchar16 *in, int len);

long ch64_request(void);
long ch64_request_40000(void);

#endif

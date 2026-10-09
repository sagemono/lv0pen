#include "qa_flag.h"
#include "cipher.h"
#include "verifier.h"
#include "util.h"

extern const unsigned char qa_flag_pub[40];
extern const unsigned int qa_flag_curve;
extern const unsigned char qa_flag_hmac_key[64];
extern const unsigned char qa_flag_key[32];
extern const unsigned char qa_flag_iv[16];

struct qa_flag_token {
    u32 version;
    unsigned char idps[16];
    unsigned char flag[32];
    unsigned char unknown[8];
    unsigned char md[20];
    unsigned char rest[48];
} __attribute__((aligned(16)));

int bytes_differ(const unsigned char *a, const unsigned char *b, int n);

void qa_flag_reader::init()
{
}

long qa_flag_reader::check(u32 flag, const unsigned char *token, unsigned char *out,
                           const unsigned char *idps, const unsigned char *sig)
{
    if (flag == 0xFFFF)
        return -3;
    if (sig != 0) {
        ecdsa_verifier ver(qa_flag_pub, qa_flag_curve);

        if (ver.verify(token, 80, sig))
            return -1;
    }

    aes256_cbc_cipher c;
    qa_flag_token t;
    unsigned char md[20];

    c.set_key(qa_flag_key, qa_flag_iv);
    if (c.decrypt(token, 128, &t))
        return -99;

    hmac_digest h;

    if (h.digest((const unsigned char *)&t, 60, md, qa_flag_hmac_key, 64))
        return -99;
    if (bytes_differ(md, t.md, 20))
        return -1;
    if (t.version != 1)
        return -2;
    if (memcmp(idps, t.idps, 16))
        return -3;
    memcpy(out, t.flag, 32);
    return 0;
}

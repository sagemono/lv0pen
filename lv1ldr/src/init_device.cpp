#include <spu_intrinsics.h>
#include "loader.h"
#include "aes.h"
#include "log.h"
#include "util.h"
#include "sb.h"

bool is_sb_product_dx(void);
bool is_sb_product_2(void);
bool is_sb_product_3(void);
bool is_sb_product_4(void);
void prng_seed(void);
int prng_random(unsigned char *out);

extern const u64 sb_mmio_base;

struct key192 {
    u32 w[6];
};

struct key192_node {
    key192 key;
    key192_node *next;
};

struct ata_cfg {
    u32 flag;
    key192 key0;
    key192 key1;
    key192 key2;
    key192 key3;
};

struct encdec_cfg {
    u32 flag;
    key192_node *keys;
};

class encdec {
public:
    encdec();
    virtual ~encdec();

    u64 enable(u64 base);
    int activate(const unsigned char *kek1, const unsigned char *kek2, const key192 *key1,
                 const u32 *key, const u32 *iv, u64 ea, u64 io, const ata_cfg *ata,
                 const encdec_cfg *enc);
};

extern const unsigned char encdec_kek1_dx[], encdec_kek2_dx[], encdec_key_dx[];
extern const unsigned char encdec_kek1_2[], encdec_kek2_2[], encdec_key_2[];
extern const unsigned char encdec_kek1_3[], encdec_kek2_3[], encdec_key_3[];
extern const unsigned char encdec_kek1_4[], encdec_kek2_4[], encdec_key_4[];

long loader::init_device_1(u64 ea, u64 io, u64 flags, const device_id *id,
                           const unsigned char *key, const vec_uchar16 *iv,
                           const vec_uchar16 *k0, const vec_uchar16 *k1,
                           const vec_uchar16 *k2, const vec_uchar16 *k3,
                           const unsigned char *x0, const unsigned char *x1,
                           const unsigned char *x2)
{
    vec_uchar16 ata0[2];
    vec_uchar16 v = *iv;
    vec_uchar16 enc1[2];
    vec_uchar16 ata1[2];
    u32 rnd[5];
    vec_uchar16 enc0[2];
    u32 gkey[4];
    u32 ata_flag;

    aes_cbc_encrypt(enc0, k0, 32, key, 256, &v);
    aes_cbc_encrypt(enc1, k2, 32, key, 256, &v);
    aes_cbc_encrypt(ata1, k1, 32, key, 256, &v);
    memset(ata0, 0, 32);
    if (id->device == 129)
        ata_flag = 226;
    else if (id->device == 160) {
        memcpy(ata1, x0, 32);
        ata_flag = 34;
        if (id->model < 1 || id->model > 5) {
            memcpy(enc0, x1, 32);
            memcpy(enc1, x2, 32);
        }
    } else if (id->device == 130 && (id->model == 143 || id->model == 144)) {
        memcpy(ata1, x0, 32);
        memcpy(enc0, x1, 32);
        memcpy(enc1, x2, 32);
        ata_flag = 34;
    } else
        ata_flag = 162;
    if (is_sb_product_4()) {
        ata_flag |= 17;
        if (id->device == 160)
            memcpy(ata0, x0, 32);
        else if (id->device == 130 && (id->model == 143 || id->model == 144))
            memcpy(ata0, x0, 32);
        else
            aes_cbc_encrypt(ata0, k3, 32, key, 256, &v);
    }
    log_debug("init_device_1 flags %08x, device %d, ata_enc_flag %08x\n", flags, id->device, ata_flag);

    {
        u32 iv0[4] = { 0 };
        const unsigned char *kek1, *kek2, *key1;
        ata_cfg ata;
        key192_node n1, n0;

        prng_seed();
        prng_random((unsigned char *)rnd);
        memcpy(gkey, rnd, 16);
        encdec e;
        if (is_sb_product_dx()) {
            kek1 = encdec_kek1_dx;
            kek2 = encdec_kek2_dx;
            key1 = encdec_key_dx;
        } else if (is_sb_product_2()) {
            kek1 = encdec_kek1_2;
            kek2 = encdec_kek2_2;
            key1 = encdec_key_2;
        } else if (is_sb_product_3()) {
            kek1 = encdec_kek1_3;
            kek2 = encdec_kek2_3;
            key1 = encdec_key_3;
        } else if (is_sb_product_4()) {
            kek1 = encdec_kek1_4;
            kek2 = encdec_kek2_4;
            key1 = encdec_key_4;
        } else
            return 38;
        e.enable(sb_mmio_base);

        memset(&ata, 0, sizeof(ata));
        ata.flag = ata_flag;
        ata.key0 = *(const key192 *)ata0;
        ata.key1 = *(const key192 *)ata1;
        memset(&n1, 0, sizeof(n1));
        n1.key = *(const key192 *)enc1;
        memset(&n0, 0, sizeof(n0));
        n0.key = *(const key192 *)enc0;
        n0.next = &n1;
        encdec_cfg enc[1] = { { 6, &n0 } };
        if (e.activate(kek1, kek2, (const key192 *)key1, gkey, iv0, ea, io, &ata, enc))
            return 38;
    }
    return 0;
}

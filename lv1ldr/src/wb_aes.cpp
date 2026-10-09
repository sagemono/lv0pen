#include "types.h"

struct wb_aes_key {
    const u32 *rk;
};

extern const unsigned char wb_aes_table_0[16][256];
extern const u32 wb_aes_table_1[16][256];
extern const unsigned char wb_aes_table_2[16][256];

int wb_aes_block(const unsigned char *in, unsigned char *out, const wb_aes_key *key)
{
    const u32 *rk;
    u32 s0, s1, s2, s3, t0, t1, t2, t3;
    int r;

    if (!in || !out || !key)
        return 0;

    rk = key->rk;
    s0 = ((u32)wb_aes_table_0[0][in[0]] << 24) | ((u32)wb_aes_table_0[1][in[1]] << 16) |
         ((u32)wb_aes_table_0[2][in[2]] << 8) | wb_aes_table_0[3][in[3]];
    s1 = ((u32)wb_aes_table_0[4][in[4]] << 24) | ((u32)wb_aes_table_0[5][in[5]] << 16) |
         ((u32)wb_aes_table_0[6][in[6]] << 8) | wb_aes_table_0[7][in[7]];
    s2 = ((u32)wb_aes_table_0[8][in[8]] << 24) | ((u32)wb_aes_table_0[9][in[9]] << 16) |
         ((u32)wb_aes_table_0[10][in[10]] << 8) | wb_aes_table_0[11][in[11]];
    s3 = ((u32)wb_aes_table_0[12][in[12]] << 24) | ((u32)wb_aes_table_0[13][in[13]] << 16) |
         ((u32)wb_aes_table_0[14][in[14]] << 8) | wb_aes_table_0[15][in[15]];
    s0 ^= rk[0];
    s1 ^= rk[1];
    s2 ^= rk[2];
    s3 ^= rk[3];

    for (r = 9; r > 0; r -= 2) {
        t0 = wb_aes_table_1[0][s0 >> 24] ^ wb_aes_table_1[5][(s1 >> 16) & 0xFF] ^
             wb_aes_table_1[10][(s2 >> 8) & 0xFF] ^ wb_aes_table_1[15][s3 & 0xFF] ^ rk[4];
        t1 = wb_aes_table_1[4][s1 >> 24] ^ wb_aes_table_1[9][(s2 >> 16) & 0xFF] ^
             wb_aes_table_1[14][(s3 >> 8) & 0xFF] ^ wb_aes_table_1[3][s0 & 0xFF] ^ rk[5];
        t2 = wb_aes_table_1[8][s2 >> 24] ^ wb_aes_table_1[13][(s3 >> 16) & 0xFF] ^
             wb_aes_table_1[2][(s0 >> 8) & 0xFF] ^ wb_aes_table_1[7][s1 & 0xFF] ^ rk[6];
        t3 = wb_aes_table_1[12][s3 >> 24] ^ wb_aes_table_1[1][(s0 >> 16) & 0xFF] ^
             wb_aes_table_1[6][(s1 >> 8) & 0xFF] ^ wb_aes_table_1[11][s2 & 0xFF] ^ rk[7];
        rk += 8;
        if (r == 1)
            break;
        s0 = wb_aes_table_1[0][t0 >> 24] ^ wb_aes_table_1[5][(t1 >> 16) & 0xFF] ^
             wb_aes_table_1[10][(t2 >> 8) & 0xFF] ^ wb_aes_table_1[15][t3 & 0xFF] ^ rk[0];
        s1 = wb_aes_table_1[4][t1 >> 24] ^ wb_aes_table_1[9][(t2 >> 16) & 0xFF] ^
             wb_aes_table_1[14][(t3 >> 8) & 0xFF] ^ wb_aes_table_1[3][t0 & 0xFF] ^ rk[1];
        s2 = wb_aes_table_1[8][t2 >> 24] ^ wb_aes_table_1[13][(t3 >> 16) & 0xFF] ^
             wb_aes_table_1[2][(t0 >> 8) & 0xFF] ^ wb_aes_table_1[7][t1 & 0xFF] ^ rk[2];
        s3 = wb_aes_table_1[12][t3 >> 24] ^ wb_aes_table_1[1][(t0 >> 16) & 0xFF] ^
             wb_aes_table_1[6][(t1 >> 8) & 0xFF] ^ wb_aes_table_1[11][t2 & 0xFF] ^ rk[3];
    }

    s0 = (wb_aes_table_1[0][t0 >> 24] & 0xFF000000) ^ (wb_aes_table_1[5][(t1 >> 16) & 0xFF] & 0x00FF0000) ^
         (wb_aes_table_1[10][(t2 >> 8) & 0xFF] & 0x0000FF00) ^ (wb_aes_table_1[15][t3 & 0xFF] & 0x000000FF) ^ rk[0];
    s1 = (wb_aes_table_1[4][t1 >> 24] & 0xFF000000) ^ (wb_aes_table_1[9][(t2 >> 16) & 0xFF] & 0x00FF0000) ^
         (wb_aes_table_1[14][(t3 >> 8) & 0xFF] & 0x0000FF00) ^ (wb_aes_table_1[3][t0 & 0xFF] & 0x000000FF) ^ rk[1];
    s2 = (wb_aes_table_1[8][t2 >> 24] & 0xFF000000) ^ (wb_aes_table_1[13][(t3 >> 16) & 0xFF] & 0x00FF0000) ^
         (wb_aes_table_1[2][(t0 >> 8) & 0xFF] & 0x0000FF00) ^ (wb_aes_table_1[7][t1 & 0xFF] & 0x000000FF) ^ rk[2];
    s3 = (wb_aes_table_1[12][t3 >> 24] & 0xFF000000) ^ (wb_aes_table_1[1][(t0 >> 16) & 0xFF] & 0x00FF0000) ^
         (wb_aes_table_1[6][(t1 >> 8) & 0xFF] & 0x0000FF00) ^ (wb_aes_table_1[11][t2 & 0xFF] & 0x000000FF) ^ rk[3];

    out[0] = wb_aes_table_2[0][s0 >> 24];
    out[1] = wb_aes_table_2[1][(s0 >> 16) & 0xFF];
    out[2] = wb_aes_table_2[2][(s0 >> 8) & 0xFF];
    out[3] = wb_aes_table_2[3][s0 & 0xFF];
    out[4] = wb_aes_table_2[4][s1 >> 24];
    out[5] = wb_aes_table_2[5][(s1 >> 16) & 0xFF];
    out[6] = wb_aes_table_2[6][(s1 >> 8) & 0xFF];
    out[7] = wb_aes_table_2[7][s1 & 0xFF];
    out[8] = wb_aes_table_2[8][s2 >> 24];
    out[9] = wb_aes_table_2[9][(s2 >> 16) & 0xFF];
    out[10] = wb_aes_table_2[10][(s2 >> 8) & 0xFF];
    out[11] = wb_aes_table_2[11][s2 & 0xFF];
    out[12] = wb_aes_table_2[12][s3 >> 24];
    out[13] = wb_aes_table_2[13][(s3 >> 16) & 0xFF];
    out[14] = wb_aes_table_2[14][(s3 >> 8) & 0xFF];
    out[15] = wb_aes_table_2[15][s3 & 0xFF];
    return 1;
}

void wb_crypt(const u32 *rk, const unsigned char *in, u32 len,
              const unsigned char *data, unsigned char *out)
{
    unsigned char ks[16];
    wb_aes_key key;
    u32 i;

    if (len > 16)
        return;
    key.rk = rk;
    wb_aes_block(in, ks, &key);
    for (i = 0; i != len; i++)
        out[i] = data[i] ^ ks[i];
}

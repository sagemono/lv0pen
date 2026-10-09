#include "aes.h"

int aes_encrypt_block(const unsigned char *in, unsigned char *out, const aes_key *key)
{
    const unsigned int *rk;
    unsigned int s0, s1, s2, s3, t0, t1, t2, t3;
    unsigned long r;
    int round;

    if (!in || !out || !key)
        return 0;

    rk = key->rk;
    s0 = ((unsigned int)aes_in_table[0][in[0]] << 24) | ((unsigned int)aes_in_table[1][in[1]] << 16) |
         ((unsigned int)aes_in_table[2][in[2]] << 8) | aes_in_table[3][in[3]];
    s1 = ((unsigned int)aes_in_table[4][in[4]] << 24) | ((unsigned int)aes_in_table[5][in[5]] << 16) |
         ((unsigned int)aes_in_table[6][in[6]] << 8) | aes_in_table[7][in[7]];
    s2 = ((unsigned int)aes_in_table[8][in[8]] << 24) | ((unsigned int)aes_in_table[9][in[9]] << 16) |
         ((unsigned int)aes_in_table[10][in[10]] << 8) | aes_in_table[11][in[11]];
    s3 = ((unsigned int)aes_in_table[12][in[12]] << 24) | ((unsigned int)aes_in_table[13][in[13]] << 16) |
         ((unsigned int)aes_in_table[14][in[14]] << 8) | aes_in_table[15][in[15]];
    s0 ^= rk[0];
    s1 ^= rk[1];
    s2 ^= rk[2];
    s3 ^= rk[3];

    r = AES_ROUNDS >> 1;
    for (round = 0; round < AES_ROUNDS;) {
        t0 = aes_round_table[round][0][s0 >> 24] ^ aes_round_table[round][5][(s1 >> 16) & 0xFF] ^
             aes_round_table[round][10][(s2 >> 8) & 0xFF] ^ aes_round_table[round][15][s3 & 0xFF] ^ rk[4];
        t1 = aes_round_table[round][4][s1 >> 24] ^ aes_round_table[round][9][(s2 >> 16) & 0xFF] ^
             aes_round_table[round][14][(s3 >> 8) & 0xFF] ^ aes_round_table[round][3][s0 & 0xFF] ^ rk[5];
        t2 = aes_round_table[round][8][s2 >> 24] ^ aes_round_table[round][13][(s3 >> 16) & 0xFF] ^
             aes_round_table[round][2][(s0 >> 8) & 0xFF] ^ aes_round_table[round][7][s1 & 0xFF] ^ rk[6];
        t3 = aes_round_table[round][12][s3 >> 24] ^ aes_round_table[round][1][(s0 >> 16) & 0xFF] ^
             aes_round_table[round][6][(s1 >> 8) & 0xFF] ^ aes_round_table[round][11][s2 & 0xFF] ^ rk[7];
        round++;
        rk += 8;
        if (--r == 0)
            break;
        s0 = aes_round_table[round][0][t0 >> 24] ^ aes_round_table[round][5][(t1 >> 16) & 0xFF] ^
             aes_round_table[round][10][(t2 >> 8) & 0xFF] ^ aes_round_table[round][15][t3 & 0xFF] ^ rk[0];
        s1 = aes_round_table[round][4][t1 >> 24] ^ aes_round_table[round][9][(t2 >> 16) & 0xFF] ^
             aes_round_table[round][14][(t3 >> 8) & 0xFF] ^ aes_round_table[round][3][t0 & 0xFF] ^ rk[1];
        s2 = aes_round_table[round][8][t2 >> 24] ^ aes_round_table[round][13][(t3 >> 16) & 0xFF] ^
             aes_round_table[round][2][(t0 >> 8) & 0xFF] ^ aes_round_table[round][7][t1 & 0xFF] ^ rk[2];
        s3 = aes_round_table[round][12][t3 >> 24] ^ aes_round_table[round][1][(t0 >> 16) & 0xFF] ^
             aes_round_table[round][6][(t1 >> 8) & 0xFF] ^ aes_round_table[round][11][t2 & 0xFF] ^ rk[3];
        round++;
    }

    s0 = (aes_round_table[round][0][t0 >> 24] & 0xFF) ^ (aes_round_table[round][5][(t1 >> 16) & 0xFF] & 0xFF000000) ^
         (aes_round_table[round][10][(t2 >> 8) & 0xFF] & 0x00FF0000) ^ (aes_round_table[round][15][t3 & 0xFF] & 0x0000FF00) ^ rk[0];
    s1 = (aes_round_table[round][4][t1 >> 24] & 0xFF) ^ (aes_round_table[round][9][(t2 >> 16) & 0xFF] & 0xFF000000) ^
         (aes_round_table[round][14][(t3 >> 8) & 0xFF] & 0x00FF0000) ^ (aes_round_table[round][3][t0 & 0xFF] & 0x0000FF00) ^ rk[1];
    s2 = (aes_round_table[round][8][t2 >> 24] & 0xFF) ^ (aes_round_table[round][13][(t3 >> 16) & 0xFF] & 0xFF000000) ^
         (aes_round_table[round][2][(t0 >> 8) & 0xFF] & 0x00FF0000) ^ (aes_round_table[round][7][t1 & 0xFF] & 0x0000FF00) ^ rk[2];
    s3 = (aes_round_table[round][12][t3 >> 24] & 0xFF) ^ (aes_round_table[round][1][(t0 >> 16) & 0xFF] & 0xFF000000) ^
         (aes_round_table[round][6][(t1 >> 8) & 0xFF] & 0x00FF0000) ^ (aes_round_table[round][11][t2 & 0xFF] & 0x0000FF00) ^ rk[3];

    out[0] = aes_out_table[12][s3 >> 24];
    out[1] = aes_out_table[13][(s3 >> 16) & 0xFF];
    out[2] = aes_out_table[14][(s3 >> 8) & 0xFF];
    out[3] = aes_out_table[15][s3 & 0xFF];
    out[4] = aes_out_table[0][s0 >> 24];
    out[5] = aes_out_table[1][(s0 >> 16) & 0xFF];
    out[6] = aes_out_table[2][(s0 >> 8) & 0xFF];
    out[7] = aes_out_table[3][s0 & 0xFF];
    out[8] = aes_out_table[4][s1 >> 24];
    out[9] = aes_out_table[5][(s1 >> 16) & 0xFF];
    out[10] = aes_out_table[6][(s1 >> 8) & 0xFF];
    out[11] = aes_out_table[7][s1 & 0xFF];
    out[12] = aes_out_table[8][s2 >> 24];
    out[13] = aes_out_table[9][(s2 >> 16) & 0xFF];
    out[14] = aes_out_table[10][(s2 >> 8) & 0xFF];
    out[15] = aes_out_table[11][s2 & 0xFF];
    return 1;
}

void aes_ctr_crypt(const unsigned int *rk, const unsigned char *ctr, unsigned int len,
                   const unsigned char *in, unsigned char *out)
{
    aes_ctx ctx;
    unsigned int i;

    if (len > 16)
        return;
    ctx.key.rk = rk;
    aes_encrypt_block(ctr, ctx.stream, &ctx.key);
    for (i = 0; i < len; i++)
        out[i] = in[i] ^ ctx.stream[i];
}

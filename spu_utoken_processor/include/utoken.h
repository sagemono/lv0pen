#ifndef SUTP_UTOKEN_H
#define SUTP_UTOKEN_H

#include "types.h"
#include "dma_channel.h"
#include "dma_stream.h"
#include "eid0_reader.h"

typedef unsigned int u128 __attribute__((mode(TI)));

struct utoken_params {
    u64 mode;
    u64 token_ea;
    u64 token_size;
    u64 out_ea;
    u64 out_size;
    u64 arg5;
    u64 arg6;
    u64 eid0_ea;
    u64 eid0_size;
    u64 arg9;
} __attribute__((aligned(16)));

#define UTOKEN_SEAL     0
#define UTOKEN_OPEN     1

#define UTOKEN_SIZE     0xC50
#define UTOKEN_MAGIC    0x73757400
#define UTOKEN_VERSION  1
#define UTOKEN_IDPS     16
#define UTOKEN_HMAC     0xC30

#define LS_READ         0x3E000
#define LS_WRITE        0x3F000
#define TAG_READ        1
#define TAG_WRITE       2

struct idps {
    u32 head;
    u16 product;
    u16 model;
    u8 rest[8];
};

class utoken {
public:
    utoken(const unsigned char *key, unsigned char *iv, const unsigned char *hmac_key,
           const unsigned char *idps);
    ~utoken();
    int put(const utoken_params *p, const unsigned char *buf);
    int get(const utoken_params *p, unsigned char *buf);
    bool check_header(const unsigned char *buf, u64 size);
    int check_idps(const unsigned char *buf);
    int seal(const utoken_params *p, unsigned char *buf);
    int open(const utoken_params *p, unsigned char *buf);

    const unsigned char *m_key;
    unsigned char *m_iv;
    const unsigned char *m_hmac_key;
    struct idps m_idps;
};

#endif

#ifndef SPTP_TOKEN_H
#define SPTP_TOKEN_H

#include "types.h"
#include "dma_channel.h"
#include "eid0_reader.h"

struct token_request {
    u64 cmd;
    u64 token_ea;
    u64 token_size;
    u64 result_ea;
    u64 result_size;
    u64 unknown_ea;
    u64 unknown_size;
    u64 eid0_ea;
    u64 eid0_size;
    u64 unused;
} __attribute__((aligned(16)));

#define CMD_UPDATE      1
#define CMD_VERIFY      2

struct qa_token {
    u32 version;
    unsigned char idps[16];
    unsigned char flag[32];
    unsigned char unknown[8];
    unsigned char md[20];
    unsigned char rest[48];
} __attribute__((aligned(16)));

class token_processor {
public:
    token_processor(const unsigned char *key, const unsigned char *iv, const unsigned char *hmac_key,
                    const unsigned char *pub, unsigned int curve, const unsigned char *idps);
    ~token_processor();
    unsigned int check_idps(const qa_token *t);
    unsigned int verify_signature(const unsigned char *token);
    unsigned int decrypt(const unsigned char *token, qa_token *t);
    unsigned int update(const token_request *req, unsigned char *out);
    unsigned int verify(const token_request *req);

    const unsigned char *m_key;
    const unsigned char *m_iv;
    const unsigned char *m_hmac_key;
    unsigned char m_idps[16];
    const unsigned char *m_pub;
    unsigned int m_curve;
};

#endif

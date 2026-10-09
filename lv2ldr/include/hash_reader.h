#ifndef LV2LDR_HASH_READER_H
#define LV2LDR_HASH_READER_H

#include "types.h"
#include "dma_stream.h"
#include "hmac_sha1.h"

class hash_reader {
public:
    hash_reader();
    virtual ~hash_reader() { }

    long open(u64 ea, u64 size, dma_buffer **bufs, unsigned int nbufs,
              const unsigned char *key, unsigned int keylen);

    long read();

    long get_digest(unsigned char *out, unsigned int n);

    long verify(const unsigned char *d, unsigned int n);

    unsigned char m_digest[20] __attribute__((aligned(16)));
    dma_queue m_queue;
    hmac_sha1_ctx m_ctx;
};

#endif

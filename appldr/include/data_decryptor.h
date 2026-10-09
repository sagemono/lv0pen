#ifndef APPLDR_DATA_DECRYPTOR_H
#define APPLDR_DATA_DECRYPTOR_H

#include "types.h"
#include "dma_stream.h"

class cipher;
class mac;

struct decryptor_state {
    unsigned int version;
    unsigned int mac_flags;
    unsigned int cipher_flags;
    unsigned int pad;
    unsigned char mac_state[240];
    unsigned char cipher_state[76];
    unsigned char md[20];
} __attribute__((aligned(16)));

class data_decryptor {
public:
    data_decryptor();

    int open(const unsigned char *mac_key, const unsigned char *key, const unsigned char *iv,
             unsigned int mac_flags, unsigned int cipher_flags, u64 state_ea, unsigned int index);

    int check(unsigned int revision, unsigned int mac_flags, unsigned int cipher_flags);

    int process(u64 src, u64 len, u64 state_ea, dma_buffer **in, unsigned int nin,
                dma_buffer **out, unsigned int nout);

    int finish(u64 ea, u64 state_ea);

    void reset();

private:
    int set_cipher(unsigned int flags);
    int set_mac(unsigned int flags);
    int restore(const decryptor_state *st);
    int save(decryptor_state *st);
    int derive_mac_key(const unsigned char *in, unsigned char *out, unsigned int flags,
                       unsigned int index);
    int derive_key(const unsigned char *key, const unsigned char *iv, unsigned char *key_out,
                   unsigned char *iv_out, unsigned int flags, unsigned int index);
    int state_keys(unsigned char *k1, unsigned char *k2);
    int seal(unsigned char *st);
    int unseal(unsigned char *st);
    int put_state(u64 ea, const unsigned char *st);
    int get_state(u64 ea, unsigned char *st);
    int pump();
    int requeue(dma_buffer *in, dma_buffer *out);

    bool m_open;
    dma_queue m_queue;
    cipher *m_cipher;
    mac *m_mac;
    bool m_in_place;
    bool m_no_verify;
    unsigned int m_mac_flags;
    unsigned int m_cipher_flags;
};

#endif

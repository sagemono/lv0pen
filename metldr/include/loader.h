#ifndef MLDR_LOADER_H
#define MLDR_LOADER_H

#include "types.h"

class authenticator;

class loader {
public:
    loader();

    long run(unsigned int *entry);

    long read_request(void);

    long load(unsigned int *entry);

    long load_header(void);

    long load_segments(unsigned int *entry);

    u64 m_ea;
    authenticator *m_auth;
};

extern loader g_loader;

extern "C" void leave(unsigned int entry);

#endif

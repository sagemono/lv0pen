#ifndef ISOLDR_QA_FLAG_H
#define ISOLDR_QA_FLAG_H

#include "types.h"

class qa_flag_reader {
public:
    void init();

    long check(u32 revision, const unsigned char *token, unsigned char *out,
               const unsigned char *idps, const unsigned char *sig);
};

#endif

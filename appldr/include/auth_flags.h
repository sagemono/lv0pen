#ifndef APPLDR_AUTH_FLAGS_H
#define APPLDR_AUTH_FLAGS_H

#include "types.h"

struct auth_flags {
    u64 v[4];
};

bool operator==(const auth_flags &a, const auth_flags &b) __attribute__((pure));
bool operator!=(const auth_flags &a, const auth_flags &b);
auth_flags operator&(const auth_flags &a, const auth_flags &b);

#endif

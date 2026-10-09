#include "verifier.h"
#include "ec.h"

extern unsigned char verify_hash[20];

ecdsa_verifier::ecdsa_verifier(const unsigned char *pub, unsigned int curve)
    : verifier(1), m_pub(pub), m_curve(curve)
{
}

int ecdsa_verifier::verify(const unsigned char *data, int len, const unsigned char *sig)
{
    if (sha1_digest(verify_hash, data, len) < 0)
        return 1;
    if (ecdsa_verify_id(sig, verify_hash, m_pub, m_curve) < 0)
        return 1;
    return 0;
}

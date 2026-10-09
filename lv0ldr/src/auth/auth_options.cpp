#include "auth.h"
#include "util.h"

void auth_state::load(const auth_entry *e)
{
    bool stop = false;

    do {
        switch (e->type) {
        case 1:
            t.a = e->v[0];
            t.b = e->v[1];
            t.c = e->v[2];
            t.d = e->v[3];
            if (e->next == 0)
                return;
            e = (const auth_entry *)((const unsigned char *)e + 48);
            break;
        case 2:
            memcpy(buf, e->v, 256);
            if (e->next == 0)
                return;
            e = (const auth_entry *)((const unsigned char *)e + 272);
            break;
        default:
            stop = true;
            break;
        }
    } while (!stop);
}

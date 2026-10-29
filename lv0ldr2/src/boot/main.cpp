#include "loader.h"

int main(void)
{
    loader l;
    long rc = l.run();

    if (rc >= 0) {
        l.finalize(false, 0);
        l.start();
    } else {
        l.finalize(true, rc);
        l.halt();
    }
    return 0;
}

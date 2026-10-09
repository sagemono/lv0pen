#include "loader.h"

int main(void)
{
    loader l;

    if (l.run() >= 0) {
        l.finalize(false);
        l.start();
    } else {
        l.finalize(true);
        l.halt();
    }
    return 0;
}

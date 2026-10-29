#include <spu_intrinsics.h>

#define ROW(k) { k, k + 1, k + 2, k + 3, k + 4, k + 5, k + 6, k + 7, \
                 k + 8, k + 9, k + 10, k + 11, k + 12, k + 13, k + 14, k + 15 }

const vec_uchar16 shuffle_table[17] = {
    ROW(0), ROW(1), ROW(2), ROW(3), ROW(4), ROW(5), ROW(6), ROW(7), ROW(8),
    ROW(9), ROW(10), ROW(11), ROW(12), ROW(13), ROW(14), ROW(15), ROW(16),
};

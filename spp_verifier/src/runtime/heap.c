struct block;

struct heap {
    struct block **rover;
    struct block *free;
};

struct heap g_heap __attribute__((aligned(16))) = { 0, 0 };

typedef unsigned int size_t;

struct block {
    size_t size;
    struct block *next;
};

struct heap {
    struct block **rover;
    struct block *free;
};

extern struct heap g_heap;

void free(void *p)
{
    struct block *b, *prev;

    if (p == 0)
        return;
    b = (struct block *)((char *)p - 16);
    if (b->size <= 15 || b->size % 16 != 0)
        return;
    if (g_heap.free == 0 || b < g_heap.free) {
        b->next = g_heap.free;
        g_heap.free = b;
    } else {
        char *end;

        prev = g_heap.free;
        while (prev->next != 0 && prev->next < b)
            prev = prev->next;
        end = (char *)prev + prev->size;
        if ((char *)b < end)
            return;
        else if (end == (char *)b) {
            prev->size += b->size;
            b = prev;
        } else if (prev->next != 0 && (char *)prev->next < (char *)b + b->size)
            return;
        else {
            b->next = prev->next;
            prev->next = b;
        }
    }
    if (b->next != 0 && (char *)b + b->size == (char *)b->next) {
        g_heap.rover = 0;
        b->size += b->next->size;
        b->next = b->next->next;
    }
}

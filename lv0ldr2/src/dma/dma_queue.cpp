#include "dma_stream.h"

long dma_queue::add_written(dma_buffer *b)
{
    if (!is_open)
        return -4;
    list_node *head = &writing;
    list_node *first = head->next;
    list_node *n = b;
    n->prev = head;
    n->next = first;
    first->prev = n;
    head->next = n;
    return 0;
}

long dma_queue::open(u64 put_ea, unsigned int put_size, u64 get_ea, unsigned int get_size)
{
    if (is_open)
        return -3;
    list_node *r = &reading;
    r->next = r;
    r->prev = r;
    list_node *w = &writing;
    w->next = w;
    w->prev = w;
    this->put_ea = put_ea;
    this->put_size = put_size;
    this->get_ea = get_ea;
    this->get_size = get_size;
    put_next = put_ea;
    put_left = put_size;
    get_next = get_ea;
    get_left = get_size;
    is_open = true;
    last_read = 0;
    return 0;
}

dma_queue::dma_queue()
{
    put_ea = 0;
    put_size = 0;
    get_ea = 0;
    get_size = 0;
    put_next = 0;
    put_left = 0;
    get_next = 0;
    get_left = 0;
    list_node *r = &reading;
    r->next = r;
    r->prev = r;
    list_node *w = &writing;
    w->next = w;
    w->prev = w;
    is_open = false;
    last_read = 0;
}

long dma_queue::enqueue_read(dma_buffer *b)
{
    unsigned int done;
    if (!is_open)
        return -4;
    if (get_left == 0)
        return 0;
    list_node *head = &reading;
    list_node *first = head->next;
    list_node *n = b;
    n->prev = head;
    n->next = first;
    first->prev = n;
    head->next = n;
    if (b->get(get_next, get_left, &done))
        return -1;
    if (done > get_left)
        return -1;
    get_next += done;
    get_left -= done;
    if (get_left == 0)
        last_read = b;
    return 0;
}

long dma_queue::enqueue_write(dma_buffer *b)
{
    unsigned int done;
    if (!is_open)
        return -4;
    if (put_left == 0)
        return 0;
    list_node *head = &writing;
    list_node *first = head->next;
    list_node *n = b;
    n->prev = head;
    n->next = first;
    first->prev = n;
    head->next = n;
    if (b->put(put_next, put_left, &done))
        return -1;
    if (done > put_left)
        return -1;
    put_next += done;
    put_left -= done;
    return 0;
}

long dma_queue::dequeue(dma_buffer **out, list_node *list, bool (dma_buffer::*done)())
{
    if (!is_open)
        return -4;
    list_node *n = list->prev;
    if (n == list)
        return -2;
    dma_buffer *b = static_cast<dma_buffer *>(n);
    b->unlink();
    while (!(b->*done)())
        ;
    *out = b;
    return 0;
}

long dma_queue::drain(list_node *list, bool (dma_buffer::*done)())
{
    dma_buffer *b;
    for (;;) {
        long rc = dequeue(&b, list, done);
        switch (rc) {
        case 0:
            break;
        case -2:
            return 0;
        default:
            return rc;
        }
    }
}

void dma_queue::close()
{
    if (drain(&reading, &dma_buffer::get_done) == 0
        && drain(&writing, &dma_buffer::put_done) == 0)
        is_open = false;
}

long dma_queue::dequeue_write(dma_buffer **out)
{
    return dequeue(out, &writing, &dma_buffer::put_done);
}

long dma_queue::dequeue_read(dma_buffer **out)
{
    long rc = dequeue(out, &reading, &dma_buffer::get_done);
    if (rc == 0 && last_read == *out)
        return 1;
    return rc;
}

list_entry::list_entry()
{
    next = this;
    prev = this;
}

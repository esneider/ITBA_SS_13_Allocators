#include "allocator.h"

#define META_EXTRA_T struct meta_extra

struct meta_extra {
    struct meta_extra *prev;
    struct meta_extra *next;
};

#include "chunk.h"

#define DATA(chunk) ((struct meta_extra*)chunk_data(chunk))
#define DATA_SIZE   (sizeof(struct meta_extra))
#define PADD_SIZE   (DATA_SIZE + head_size())


static inline void delete_node(struct meta_extra *node) {

    node->next->prev = node->prev;
    node->prev->next = node->next;
}


void alloc_init(size_t size, void *heap) {

    init_heap(size, heap);

    struct chunk *begin = begin_chunk();

    info->extra.next = DATA(begin);
    info->extra.prev = NULL;

    DATA(begin)->next = NULL;
    DATA(begin)->prev = &info->extra;
}


void *alloc_malloc(size_t size) {

    if (size < DATA_SIZE) size = DATA_SIZE;

    for (struct meta_extra *e = &(info->extra); e->next; e = e->next) {

        struct chunk *c1 = data_chunk(e->next);

        if (c1->next_size >= size) {

            if (c1->next_size >= size + PADD_SIZE) {

                struct chunk *c2 = split_chunk(c1, size);
                DATA(c2)->next = DATA(c1)->next;
                DATA(c2)->prev = DATA(c1);
                DATA(c2)->next->prev = DATA(c1)->next = DATA(c2);
            }

            delete_node(DATA(c1));
            use_chunk(c1);
            return DATA(c1);
        }
    }

    return NULL;
}


void alloc_free(void *mem) {

    if (!mem) return;

    struct chunk *c = data_chunk(mem);
    free_chunk(c);

    if (is_free_prev_chunk(c)) {

        delete_node(DATA(prev_chunk(c)));
        c = coalesce_chunk(c);
    }

    if (is_free_chunk(next_chunk(c))) {

        delete_node(DATA(next_chunk(c)));
        coalesce_chunk(next_chunk(c));
    }

    DATA(c)->next = info->extra.next;
    DATA(c)->prev = &(info->extra);
    info->extra.next = DATA(c);
}

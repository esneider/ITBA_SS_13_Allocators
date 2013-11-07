#include "allocator.h"
#include "list.h"
#define META_EXTRA_T struct list_head
#include "chunk.h"


#define DATA(chunk) ((struct list_head*)chunk_data(chunk))
#define DATA_SIZE   (sizeof(struct list_head))


void alloc_init(size_t size, void *heap) {

    init_heap(size, heap);

    INIT_LIST_HEAD(&info->extra);
    list_add(DATA(begin_chunk()), &info->extra);
}


void *alloc_malloc(size_t size) {

    if (size < DATA_SIZE) size = DATA_SIZE;

    list_for_each(extra, &info->extra) {

        struct chunk *c = data_chunk(extra);

        if (c->curr_size >= size) {

            if (c->curr_size >= size + DATA_SIZE + head_size()) {

                list_add(DATA(split_chunk(c, size)), DATA(c));
            }

            list_del(DATA(c));
            use_chunk(c);
            return DATA(c);
        }
    }

    return NULL;
}


void alloc_free(void *mem) {

    if (!mem) return;

    struct chunk *c = data_chunk(mem);
    free_chunk(c);

    if (is_free_prev_chunk(c)) {

        list_del(DATA(prev_chunk(c)));
        c = coalesce_chunk(c);
    }

    if (is_free_chunk(next_chunk(c))) {

        list_del(DATA(next_chunk(c)));
        coalesce_chunk(next_chunk(c));
    }

    list_for_each(extra, &info->extra) {

        if (data_chunk(extra)->curr_size >= c->curr_size) {

            list_add(DATA(c), extra->prev);
            return;
        }
    }

    list_add(DATA(c), info->extra.prev);
}

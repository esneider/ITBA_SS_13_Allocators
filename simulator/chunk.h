#ifndef __CHUNK_H__
#define __CHUNK_H__

#include <stddef.h>
#include <stdbool.h>
#include <assert.h>


#define FREE 0
#define USED 1


struct chunk {

    size_t prev_size: 31;
    size_t prev_state: 1;
    size_t next_size: 31;
    size_t next_state: 1;

#ifdef CHUNK_EXTRA_T
    CHUNK_EXTRA_T extra;
#endif
};

struct info {

    void *heap;
    size_t heap_size;

    void *arena;
    size_t arena_size;

    struct chunk* begin;
    struct chunk* end;

#ifdef META_EXTRA_T
    META_EXTRA_T extra;
#endif
};

struct stats {

   /* Mesures the heap external fragmentation:
      rel_ext_frag = 1 - larger free block / total free memory */
    double rel_ext_frag;

   /* Mesures the ratio of metadata memory vs total memory:
      rel_metadata = total metadata size / total memory */
    double rel_metadata;
};


/*
 * Globals
 */

static struct info *info;

/*
 * Chunk types
 */

static inline bool is_free_chunk(struct chunk *chunk) {

    return chunk->next_state == FREE;
}

static inline bool is_used_chunk(struct chunk *chunk) {

    return chunk->next_state == USED;
}

static inline bool is_free_prev_chunk(struct chunk *chunk) {

    return chunk->prev_state == FREE;
}

static inline bool is_used_prev_chunk(struct chunk *chunk) {

    return chunk->prev_state == USED;
}

/*
 * Chunk sizes
 */

static inline size_t size_marker(void) {

    return (1L << 31) - 1;
}

static inline size_t head_size(void) {

    return sizeof(struct chunk);
}

/*
 * Chunks data
 */

static inline void *chunk_data(struct chunk *chunk) {

    return (char*)chunk + head_size();
}

static inline void *prev_chunk_data(struct chunk *chunk) {

    return (char*)chunk - chunk->prev_size;
}

static inline struct chunk *data_chunk(void *data) {

    return (void*)((char*)data - head_size());
}

#ifdef CHUNK_EXTRA_T
static inline CHUNK_EXTRA_T *chunk_extra(struct chunk *chunk) {

    return &chunk->extra;
}
#endif

/*
 * Chunks navigation
 */

static inline bool is_begin_chunk(struct chunk *chunk) {

    return chunk->prev_size == size_marker();
}

static inline bool is_end_chunk(struct chunk *chunk) {

    return chunk->next_size == size_marker();
}

static inline struct chunk *next_chunk(struct chunk *chunk) {

    assert(!is_end_chunk(chunk));

    return (void*)((char*)chunk_data(chunk) + chunk->next_size);
}

static inline struct chunk *prev_chunk(struct chunk *chunk) {

    assert(!is_begin_chunk(chunk));

    return data_chunk(prev_chunk_data(chunk));
}

static inline struct chunk *begin_chunk(void) {

    return info->begin;
}

static inline struct chunk *first_chunk(void) {

    return begin_chunk();
}

static inline struct chunk *end_chunk(void) {

    return info->end;
}

static inline struct chunk *last_chunk(void) {

    return prev_chunk(end_chunk());
}

static inline bool is_first_chunk(struct chunk *chunk) {

    return is_begin_chunk(chunk);
}

static inline bool is_last_chunk(struct chunk *chunk) {

    assert(!is_end_chunk(chunk));

    return is_end_chunk(next_chunk(chunk));
}

/*
 * Chunk operations
 */

static struct chunk *split_chunk(struct chunk *chunk, size_t size) {

    assert(is_free_chunk(chunk));

    struct chunk *chunk1 = chunk;
    struct chunk *chunk2 = (void*)((char*)chunk_data(chunk1) + size);
    struct chunk *chunk3 = next_chunk(chunk1);
    size_t size2 = (void*)chunk3 - chunk_data(chunk2);

    chunk2->next_size = chunk3->prev_size = size2;
    chunk1->next_size = chunk2->prev_size = size;

    chunk2->prev_state = FREE;
    chunk2->next_state = chunk3->next_state;

    return chunk2;
}

static struct chunk *coalesce_chunk(struct chunk *chunk) {

    assert(is_free_chunk(chunk));
    assert(is_free_prev_chunk(chunk));

    struct chunk *chunk1 = prev_chunk(chunk);
    struct chunk *chunk2 = chunk;
    struct chunk *chunk3 = next_chunk(chunk2);
    size_t size = (void*)chunk3 - chunk_data(chunk1);

    chunk1->next_size = chunk3->prev_size = size;

    return chunk1;
}

static inline void free_chunk(struct chunk *chunk) {

    assert(is_used_chunk(chunk));
    assert(!is_end_chunk(chunk));

    next_chunk(chunk)->prev_state = chunk->next_state = FREE;
}

static inline void use_chunk(struct chunk *chunk) {

    assert(is_free_chunk(chunk));

    next_chunk(chunk)->prev_state = chunk->next_state = USED;
}

/*
 * Heap operations
 */

static void init_heap(size_t size, void *heap) {

    info = heap;

    info->heap = heap;
    info->heap_size = size;

    info->arena = info + 1;
    info->arena_size = (void*)((char*)heap + size) - info->arena;

    info->begin = info->arena;
    info->begin->prev_state = USED;
    info->begin->next_state = FREE;
    info->begin->prev_size = size_marker();
    info->begin->next_size = info->arena_size - 2 * head_size();

    info->end = next_chunk(info->begin);
    info->end->prev_state = FREE;
    info->end->next_state = USED;
    info->end->prev_size = info->begin->next_size;
    info->end->next_size = size_marker();
}

struct stats alloc_stats(void) {

    struct stats stats;
    struct chunk *chunk = begin_chunk();

    size_t meta_size = info->heap_size;
    size_t free_size = 0;
    size_t largest = 0;

    for ( ; !is_end_chunk(chunk); chunk = next_chunk(chunk)) {

        meta_size -= chunk->next_size;

        if (is_free_chunk(chunk)) {

            free_size += chunk->next_size;

            if (chunk->next_size > largest) {

                largest = chunk->next_size;
            }
        }
    }

    stats.rel_ext_frag = largest / (double)free_size;
    stats.rel_metadata = meta_size / (double)info->heap_size;

    return stats;
}


#undef CHUNK_EXTRA_T
#undef META_EXTRA_T
#undef FREE
#undef USED

#endif /* __CHUNK_H__ */

#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include <stddef.h>

struct stats;


/**
 * Initialize the heap with a block of size bytes.
 *
 * @param size - Size of the heap.
 * @param heap - Memory heap.
 */
void alloc_init(size_t size, void *heap);

/**
 * Request a block of memory.
 *
 * @param size - Minimum size of the block.
 * @returns a pointer to the start of the block.
 */
void *alloc_malloc(size_t size);

/**
 * Return a block of memory.
 *
 * @param mem - pointer to the start of the block.
 */
void alloc_free(void *mem);

/**
 * Get allocator statistics.
 *
 * @returns a stats structure.
 */
struct stats alloc_stats(void);


#endif /* __ALLOCATOR_H__ */

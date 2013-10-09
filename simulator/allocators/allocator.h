#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include <stddef.h>


/**
 * Initialize the heap with a block of size bytes.
 *
 * @param size - Size of the heap.
 */
void init(size_t size);

/**
 * Request a block of memory.
 *
 * @param size - Minimum size of the block.
 * @returns a pointer to the start of the block.
 */
void *malloc(size_t size);

/**
 * Return a block of memory.
 *
 * @param mem - pointer to the start of the block.
 */
void free(void *mem);


#endif /* __ALLOCATOR_H__ */

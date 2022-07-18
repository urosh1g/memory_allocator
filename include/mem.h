#include <stdio.h>

#define ALIGNMENT 2
#define align(x) ((x + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define DEBUG 1
#define debug_print(fmt, ...) do { if(DEBUG) fprintf(stdout, fmt, __VA_ARGS__); } while(0)
#define HEADER_SIZE (align(sizeof(header_t)))
#define HEAP_CAPACITY 64000

/*
 * Header struct containing
 * the block size, and using
 * the lowest bit of `size` as
 * allocation bit
 */
typedef struct {
	size_t size;
} header_t, *pheader_t;

unsigned char heap[HEAP_CAPACITY] = {0};
unsigned char* HEAP_START = NULL;
unsigned char* HEAP_END = heap + HEAP_CAPACITY - HEADER_SIZE;

/*
 * Initialize the heap for
 * future allocations
 */
void mem_init();

/*
 * Find a block inside the heap
 * that can hold `size` bytes and
 * return a pointer to its header
 * #First Fit algorithm
 */
pheader_t find_block(const size_t size);

/*
 * Allocate at least `size` bytes
 * on the heap, and return a
 * unique pointer to allocated space
 */
void* mem_alloc(size_t size);

/*
 * Free the memory pointed
 * by `ptr` if it's a valid
 * pointer returned by mem_alloc
 */
void mem_free(void* ptr);


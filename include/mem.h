#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ALIGNMENT 2
#define align(x) ((x + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define DEBUG 0
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
pheader_t HEAP_START = NULL;
pheader_t HEAP_END = (pheader_t)(heap + HEAP_CAPACITY - HEADER_SIZE);

/*
 * Checks if the block is free
 */
bool is_free(pheader_t block_header);

void handle_error(const char* msg);

/*
 * Debug information about 
 * the heap
 */
void print_blocks();

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



void mem_init(){
	HEAP_START = (pheader_t)heap;
	HEAP_START->size = align(HEAP_CAPACITY - HEADER_SIZE);
}

/*
 * Also doing deferred coalescing,
 * if the size can't fit in current block (header)
 * and the next block is free, merge them and try again
 */
pheader_t find_block(const size_t size){
	pheader_t header = HEAP_START;
	pheader_t next_header = NULL;
	while(header < HEAP_END){
		if(is_free(header)){
			if(header->size >= size)
				return header;
			next_header = (pheader_t)((char*)header + header->size);
			if(next_header < HEAP_END && is_free(next_header)){
				header->size += next_header->size;
				continue;
			}
		}
		else {
			next_header = (pheader_t)((char*)header + (header->size & ~1L));
		}
		header = next_header;
	}
	return NULL;
}

void* mem_alloc(size_t size){
	if(!HEAP_START)
		mem_init();
	if(size < 2) 
		handle_error("Minimum allocation size");
	size = align(size + HEADER_SIZE);
	pheader_t header = find_block(size);
	if(!header)
		handle_error("Could not find a fitting block");
	if(header->size > size){
		pheader_t next = (pheader_t)((char*)header + size);
		next->size = align(header->size - size);
	}
	header->size = size | 1;
	return (char*)header + HEADER_SIZE;
}

bool is_free(pheader_t block_header){
	return !(block_header->size & 1);
}

void handle_error(const char* msg){
	fprintf(stderr, "Error: %s\n", msg);
	exit(-1);
}

void print_blocks(){
	pheader_t header = HEAP_START;
	while(header < HEAP_END){
		size_t size = header->size - 1;
		if(is_free(header)){
			size = header->size;
			printf("Not allocated ");
		}
		else 
			printf("Allocated ");
		printf("header at: %p, with block size %lu\n", (void*)header, size);
		header = (pheader_t)((char*)header + size);
	}
}

void mem_free(void* ptr){
	if(ptr < (void*)HEAP_START || ptr > (void*)HEAP_END)
		handle_error("mem_free requires a valid pointer returned by mem_alloc\n");
	pheader_t header = (pheader_t)((char*)ptr - HEADER_SIZE);
	header->size &= ~1L;
}

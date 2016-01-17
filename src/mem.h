#ifndef _MEM_H_
#define _MEM_H_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>

#define DEBUG_FIRST_BYTES 30
#define PAGE_LENGTH 4096
#define HEAP_START ((void*)0x04040000)

#pragma pack(push, 1)
struct mem_t{
	struct mem_t* next;
	size_t capacity;
	int is_free;
};
#pragma pack(pop)

typedef struct mem_t mem_t;

void* _malloc(const size_t len);
void _free( void* vmem );
void heap_init();
void memalloc_debug_struct_info( FILE* f, mem_t const* const address );
void memalloc_debug_heap( FILE* f );

void swap_next(mem_t* mem);
void unite_next(mem_t* mem);

#endif

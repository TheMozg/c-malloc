#include "mem.h"

mem_t* find_last(){
	mem_t* cur = HEAP_START;
	while (cur->next != NULL)
		cur = cur->next;
	return cur;
}

void split_block(mem_t* mem, size_t n){
	mem_t* free_mem = (mem_t*)((uint8_t*) mem + n + sizeof(mem_t));
	free_mem->capacity = mem->capacity - n - sizeof(mem_t);
	free_mem->is_free = 1;
	free_mem->next = NULL;
	mem->next = free_mem;
	mem->capacity = n;
}

void attach_block(mem_t* next_mem, mem_t* mem, size_t length){
	mem->next = next_mem;
	next_mem->next = NULL;
	next_mem->is_free = 1;
	next_mem->capacity = (length/PAGE_LENGTH + 1)*PAGE_LENGTH - sizeof(mem_t);
	split_block(next_mem, length);
}

void* create_block(size_t length){
	mem_t* last = find_last();
	mem_t* mem = mmap( (void*)((uint8_t*)last + last->capacity + sizeof(mem_t)) , length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, 0, 0 );
	if ((uint8_t*)mem != (uint8_t*)last + last->capacity + sizeof(mem_t)) {
		attach_block(mem, last, length);
		return mem;
	} else {
		if (last->is_free){
			last->capacity = (length/PAGE_LENGTH)*PAGE_LENGTH+last->capacity;
			split_block(last, length);
			return last;
		} else {
			attach_block(mem, last, length);
			return mem;
		}
	}
}

void* find_block(size_t n){
	mem_t* cur = HEAP_START;
	while (cur != NULL)	{
		if (cur->capacity >= n && cur->is_free)
			return cur;
		cur = cur->next;
	}
	return NULL;
}

void* _malloc(const size_t length) {
	mem_t* mem = find_block(length);
	if (mem) {
		if (mem->capacity >= length + sizeof(mem_t) + 5) {
			split_block(mem, length);
		}
	}
	else {
		mem = create_block(length);
	}
	mem->is_free = 0;
	return (void*)((uint8_t*)mem + sizeof(mem_t));
}

void unite_next(mem_t* mem){
	mem_t* next_mem = mem->next;
	mem->capacity += next_mem->capacity + sizeof(mem_t);
	mem->next = next_mem->next;
}

/*
moves 'length' bytes starting from 'oldstart' to 'newstart'
memory regions may overlap, however:
'newstart' must be less than 'oldstart'
if 'newstart' > 'oldstart' behavior is undefined
*/
void _memmove(uint8_t* newstart, uint8_t* oldstart, size_t length){
	size_t i;
	for (i = 0; i < length; i++) {
		*(newstart+i) = *(oldstart+i);
	}
}

/*
swaps next block with argument block
argument block becomes free and its payload becomes undefined
*/
void swap_next(mem_t* mid){
	if(mid->next == NULL)
		return;
	mem_t* next = mid->next;
	mem_t* nextnext = next->next;
	size_t block_size = mid->capacity + sizeof(mem_t);
	_memmove((uint8_t*)mid, (uint8_t*)next, next->capacity + sizeof(mem_t));
	mid->capacity += block_size;
	split_block(mid, mid->capacity - block_size);
	mid->next->next = nextnext;
}

void _free( void* mem ){
	mem_t* cur = HEAP_START;
	mem_t* fmem = (mem_t*)((uint8_t*) mem - sizeof(mem_t));
	fmem->is_free = 1;
	if (fmem->next != NULL && fmem->next->is_free) {
		unite_next(fmem);
	}
	if(fmem != HEAP_START){
		while (cur->next != fmem)
			cur = cur->next;
		if(cur->is_free)
			unite_next(cur);
	}
}

void heap_init(){
	mem_t* mem = mmap( HEAP_START, PAGE_LENGTH, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, 0, 0 );
	mem->next = NULL;
	mem->capacity = PAGE_LENGTH - sizeof(mem_t);
	mem->is_free = 1;
}

void memalloc_debug_struct_info( FILE* f, mem_t const* const address ) {
	size_t i;
	fprintf( f, "start: %p\nsize: %lu\nis_free: %d\n", (void*)address, address->capacity, address->is_free );
	fprintf( f, "first %d bytes: ", DEBUG_FIRST_BYTES );
	for ( i = 0; i < DEBUG_FIRST_BYTES && i < address->capacity; ++i )
		fprintf( f, "%hhX", ((uint8_t*)address)[ sizeof( mem_t ) + i ] );
	putc( '\n', f );
	putc( '\n', f );
}

void memalloc_debug_heap( FILE* f ) {
	mem_t* ptr = ((mem_t*)0x04040000);
	puts("");
	for( ; ptr; ptr = ptr->next ) memalloc_debug_struct_info( f, ptr );
}

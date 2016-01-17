#include "mem.h"
#include "imem.h"

void set_int8(size_t id, int8_t value){
	*((int8_t*)obj(id)) = value;
}

int main(int argc, char** argv){

	size_t id1;
	size_t id2;
	size_t id3;
	size_t id4;
	size_t id5;

	heap_init();
	puts("INITIALIZE HEAP");
	memalloc_debug_heap(stdout);

	id1 = new(sizeof(int8_t));
	set_int8(id1, 1);
	id2 = new(sizeof(int8_t));
	set_int8(id2, 2);
	id3 = new(sizeof(int8_t));
	set_int8(id3, 3);
	id4 = new(sizeof(int8_t));
	set_int8(id4, 4);
	id5 = new(sizeof(int8_t));
	set_int8(id5, 5);
	puts("\nALLOCATE 5 INTEGERS");
	memalloc_debug_heap(stdout);

	delete(id2);
	delete(id4);
	puts("\nDELETE 2 INTEGERS");
	memalloc_debug_heap(stdout);

	defragment();
	puts("\nDEFRAGMENT HEAP");
	memalloc_debug_heap(stdout);

	set_int8(id5, 9);
	puts("\nASSIGN NEW VALUE");
	memalloc_debug_heap(stdout);

	return 0;
}

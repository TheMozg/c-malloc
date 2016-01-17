#include "imem.h"

void* objval[MAPSIZE];
size_t n = 0;

void* obj(size_t id){
  if(id < 0 || id >= MAPSIZE)
    return NULL;
  return objval[id];
}

size_t new(const size_t len){
  if(n == MAPSIZE)
    return -1;
  size_t id = n;
  objval[n] = _malloc(len);
  n++;
	return id;
}

void delete(size_t id){
  if(id < 0 || id >= MAPSIZE)
    return;
  _free(obj(id));
  objval[id] = NULL;
}

void defragment(){
  size_t i;
  mem_t* cur = HEAP_START;
  while (!cur->is_free && cur->next != NULL)
    cur = cur->next;
  while(cur->next != NULL){
    if(cur->next->is_free){
      unite_next(cur);
    } else {
      for(i = 0; i < MAPSIZE; i++){
        mem_t* mem = (mem_t*)((uint8_t*) objval[i] - sizeof(mem_t));
        if(mem == cur->next){
          swap_next(cur);
          objval[i] = (void*)((uint8_t*)cur + sizeof(mem_t));
          break;
        }
      }
      cur = cur->next;
    }
  }
}

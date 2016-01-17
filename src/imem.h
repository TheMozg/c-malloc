#ifndef _IMEM_H_
#define _IMEM_H_

#include "mem.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define MAPSIZE 20

extern void* objval[MAPSIZE];
extern size_t n;

void* obj(size_t id);

size_t new(const size_t len);

void delete(size_t id);

void defragment();

#endif

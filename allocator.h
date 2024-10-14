#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstddef>
#include <iostream>
#include <list>

struct Allocation {
    std::size_t size;
    void *space;
};

// Global variables to track allocated and free memory
std::list<Allocation> allocatedList; // List of allocated chunks
std::list<Allocation> freeList;      // List of free chunks

void *alloc(std::size_t chunk_size);
void dealloc(void *chunk);
void printMemoryLists();
void setStrategy(const std::string& strategy);
void initializeFreeList();

#endif // ALLOCATOR_H

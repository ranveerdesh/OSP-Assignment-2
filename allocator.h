#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstddef>
#include <iostream>
#include <list>
#include <string>

// Struct for memory allocation
struct Allocation {
    std::size_t totalSize;
    std::size_t usedSize;
    void* space;
};

// Global variables
extern std::list<Allocation> allocatedList;
extern std::list<Allocation> freeList;
extern std::string currentStrategy;

// Function prototypes
void* alloc(std::size_t chunk_size);
void dealloc(void* chunk);
void setStrategy(const std::string& strategy);
void initializeFreeList();
void printMemoryLists();

#endif // ALLOCATOR_H

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstddef>
#include <iostream>
#include <vector>

// Struct for memory allocation
struct Allocation {
    std::size_t totalSize;
    std::size_t usedSize;
    void* space;
};

// Global variables
extern std::vector<Allocation> allocatedList;
extern std::vector<Allocation> freeList;

// Function prototypes
void* alloc(std::size_t chunk_size);
void dealloc(void* chunk);
void setStrategy(const std::string& strategy);
void initializeFreeList();
void printMemoryLists();

#endif // ALLOCATOR_H

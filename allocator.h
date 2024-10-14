#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <iostream>
#include <vector>

// Structure for memory allocation
struct Allocation {
    std::size_t size;
    void *space;
};

// Function declarations
void *alloc(std::size_t chunk_size);
void dealloc(void *chunk);
void initializeFreeList();
void printMemoryLists();
void setStrategy(const std::string& strat);

// Global variable declarations
extern std::vector<Allocation> allocatedList; // Use 'extern' here
extern std::vector<Allocation> freeList; // Use 'extern' here

#endif // ALLOCATOR_H

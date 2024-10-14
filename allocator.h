#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstddef>
#include <iostream>
#include <list>

// Struct to represent memory allocations
struct allocation {
    std::size_t size;  // Size of the allocated chunk
    void *space;       // Pointer to the allocated memory
};

// Function declarations
void initMemoryManagement();
void* alloc(std::size_t chunk_size);
void dealloc(void *chunk);
void printMemoryLists();
void setAllocationStrategy(const std::string& strategy); // Set allocation strategy (first fit or best fit)

#endif // ALLOCATOR_H

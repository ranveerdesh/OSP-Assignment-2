#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstddef>
#include <iostream>
#include <list>
#include <string>

// Struct for representing memory allocation details
struct MemoryAllocation {
    std::size_t totalSize; // Total size of the allocated chunk
    std::size_t usedSize;  // Currently used size within the chunk
    void* memoryAddress;    // Address of the allocated memory
};

// Global variables to track allocated and free memory chunks
extern std::list<MemoryAllocation> allocatedList; // List of currently allocated chunks
extern std::list<MemoryAllocation> freeList;      // List of available free chunks
extern std::string currentStrategy;                // Current memory allocation strategy

// Function prototypes
void* alloc(std::size_t chunk_size);               // Allocate memory
void dealloc(void* chunk);                         // Deallocate memory
void setStrategy(const std::string& strategy);     // Set memory allocation strategy
void initializeFreeList();                         // Initialize the free list
void printMemoryLists();                           // Print allocated and free memory lists

#endif // ALLOCATOR_H

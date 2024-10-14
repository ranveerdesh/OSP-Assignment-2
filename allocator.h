#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <cstddef>
#include <iostream>
#include <list>
#include <string>

// Structure representing a memory allocation
struct MemoryAllocation {
    std::size_t totalSize;   // Total size of the allocation
    std::size_t usedSize;    // Size currently in use
    void* memoryAddress;      // Pointer to the allocated memory
};

// Global variables to manage allocations
extern std::list<MemoryAllocation> allocatedList; // List of currently allocated memory chunks
extern std::list<MemoryAllocation> freeList;      // List of available memory chunks
extern std::string allocationStrategy;             // Current memory allocation strategy

// Function prototypes for memory management
void* alloc(std::size_t requestedSize);            // Allocate memory of the specified size
void dealloc(void* chunk);                         // Deallocate previously allocated memory
void setStrategy(const std::string& strategy);     // Set the memory allocation strategy
void initializeFreeList();                         // Initialize the list of free memory chunks
void printMemoryLists();                           // Print the state of allocated and free memory

#endif // MEMORY_ALLOCATOR_H

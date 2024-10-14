#include "allocator.h"

// Initialize global variables
Node *allocatedList = nullptr;
Node *freeList = nullptr;

void initMemoryManagement() {
    // Initialize memory management (this could set up your free list, etc.)
}

void* alloc(std::size_t chunk_size) {
    // Implementation of memory allocation
    std::cerr << "Memory allocation failed!" << std::endl; // This should now work
    return nullptr;
}

void dealloc(void* chunk) {
    // Implementation of memory deallocation
    std::cerr << "Error: Attempting to deallocate a chunk that was not allocated!" << std::endl; // This should now work
}

void printMemoryLists() {
    // Implementation of printing the memory lists
    std::cout << "Allocated List:" << std::endl; // This should now work
}

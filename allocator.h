#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstddef>

// Struct for memory allocation
struct allocation {
    std::size_t size;
    void *space;
};

// Node structure for linked list
struct Node {
    allocation allocation;
    Node *next;
};

// Function declarations
void initMemoryManagement();
void *alloc(std::size_t chunk_size);
void dealloc(void *chunk);
void printMemoryLists();

#endif // ALLOCATOR_H

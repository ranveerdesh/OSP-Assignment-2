#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <iostream> // Include for std::cout and std::cerr
#include <vector>   // Include if you are using std::vector

// Structure to represent an allocation
struct Allocation {
    std::size_t size; // Size of the allocated memory
    void *space;      // Pointer to the allocated memory
};

// Node structure for linked lists
struct Node {
    Allocation alloc; // Change 'allocation' to 'alloc'
    Node *next;      // Pointer to the next node
};

// Global variables
extern Node *allocatedList; // List of allocated memory chunks
extern Node *freeList;      // List of free memory chunks

// Function prototypes
void initMemoryManagement(); // Initialize memory management
void* alloc(std::size_t chunk_size); // Allocate memory
void dealloc(void* chunk); // Deallocate memory
void printMemoryLists(); // Print the lists of allocated and free chunks

#endif // ALLOCATOR_H

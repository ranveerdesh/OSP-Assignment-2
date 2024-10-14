// allocator.cpp
#include "allocator.h"
#include <unistd.h> // For sbrk()
#include <cstdlib>  // For std::exit

// Global pointers for free and allocated chunks
Node *freeList = nullptr;
Node *allocatedList = nullptr;

// Function to initialize memory management
void initMemoryManagement() {
    freeList = nullptr;
    allocatedList = nullptr;
}

// Function to allocate memory
void *alloc(std::size_t chunk_size) {
    // Align chunk size to a fixed partition size
    const std::size_t partitionSizes[] = {32, 64, 128, 256, 512};
    std::size_t allocSize = 0;

    // Find the suitable partition size
    for (size_t size : partitionSizes) {
        if (chunk_size <= size) {
            allocSize = size;
            break;
        }
    }
    
    // Search for a free chunk
    Node *current = freeList;
    Node *prev = nullptr;

    while (current != nullptr) {
        if (current->allocation.size >= allocSize) { // Found a suitable free chunk
            // Remove it from the free list
            if (prev) {
                prev->next = current->next;
            } else {
                freeList = current->next; // Update head of free list
            }

            // Add it to the allocated list
            current->next = allocatedList; // Point to current allocated list
            allocatedList = current; // Update head of allocated list

            return current->allocation.space; // Return the address of the allocated chunk
        }
        prev = current;
        current = current->next;
    }

    // If no suitable chunk is found, request more memory from the OS
    void *newChunk = sbrk(allocSize);
    if (newChunk == (void *) -1) {
        std::cerr << "Memory allocation failed!" << std::endl;
        std::exit(1); // Exit on allocation failure
    }

    // Create a new node for the allocated list
    Node *newAllocation = new Node;
    newAllocation->allocation.size = allocSize;
    newAllocation->allocation.space = newChunk;
    newAllocation->next = allocatedList;
    allocatedList = newAllocation;

    return newAllocation->allocation.space; // Return the address of the new chunk
}

// Function to deallocate memory
void dealloc(void *chunk) {
    Node *current = allocatedList;
    Node *prev = nullptr;

    // Search for the chunk in the allocated list
    while (current != nullptr) {
        if (current->allocation.space == chunk) { // Found the chunk to deallocate
            // Remove from allocated list
            if (prev) {
                prev->next = current->next;
            } else {
                allocatedList = current->next; // Update head of allocated list
            }

            // Add it back to the free list
            current->next = freeList; // Point to current free list
            freeList = current; // Update head of free list

            return; // Deallocation successful
        }
        prev = current;
        current = current->next;
    }

    // If the chunk is not found, print an error message
    std::cerr << "Error: Attempting to deallocate a chunk that was not allocated!" << std::endl;
}

// Function to print the allocated and free lists for debugging
void printMemoryLists() {
    std::cout << "Allocated List:" << std::endl;
    for (Node *current = allocatedList; current != nullptr; current = current->next) {
        std::cout << "Address: " << current->allocation.space << ", Size: " << current->allocation.size << std::endl;
    }

    std::cout << "Free List:" << std::endl;
    for (Node *current = freeList; current != nullptr; current = current->next) {
        std::cout << "Address: " << current->allocation.space << ", Size: " << current->allocation.size << std::endl;
    }
}

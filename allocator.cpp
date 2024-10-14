#include "allocator.h"
#include <unistd.h>
#include <vector>
#include <stdexcept>

// Linked lists to manage allocations and free chunks
std::list<allocation> allocatedList;
std::list<allocation> freeList;

// Pointer to the current heap location
void* currentHeap = sbrk(0);

// Initialize memory management system
void initMemoryManagement() {
    // Initialize free list with fixed partition sizes
    std::vector<std::size_t> sizes = {32, 64, 128, 256, 512};
    for (const auto& size : sizes) {
        allocation freeChunk = {size, nullptr};
        freeList.push_back(freeChunk);
    }
}

// Allocate memory
void* alloc(std::size_t chunk_size) {
    // Find a suitable chunk in the free list
    for (auto it = freeList.begin(); it != freeList.end(); ++it) {
        if (it->size >= chunk_size) {
            // Allocate memory from the free chunk
            void *ptr = sbrk(it->size);
            if (ptr == (void*)-1) {
                std::cerr << "Memory allocation failed!" << std::endl;
                return nullptr; // Allocation failed
            }

            // Create allocation record
            allocation allocChunk = {it->size, ptr};
            allocatedList.push_back(allocChunk);
            freeList.erase(it); // Remove from free list
            return ptr; // Return pointer to allocated memory
        }
    }

    // If no suitable chunk is found, request more memory from OS
    void *ptr = sbrk(chunk_size);
    if (ptr == (void*)-1) {
        std::cerr << "Memory allocation failed!" << std::endl;
        return nullptr; // Allocation failed
    }

    // Record the allocation
    allocation allocChunk = {chunk_size, ptr};
    allocatedList.push_back(allocChunk);
    return ptr; // Return pointer to allocated memory
}

// Deallocate memory
void dealloc(void *chunk) {
    for (auto it = allocatedList.begin(); it != allocatedList.end(); ++it) {
        if (it->space == chunk) {
            // Move allocation to free list
            freeList.push_back(*it);
            allocatedList.erase(it); // Remove from allocated list
            return; // Successfully deallocated
        }
    }

    std::cerr << "Error: Attempting to deallocate a chunk that was not allocated!" << std::endl;
}

// Print memory lists
void printMemoryLists() {
    std::cout << "Allocated List:" << std::endl;
    for (const auto& alloc : allocatedList) {
        std::cout << "Address: " << alloc.space << ", Size: " << alloc.size << std::endl;
    }

    std::cout << "Free List:" << std::endl;
    for (const auto& free : freeList) {
        std::cout << "Address: " << free.space << ", Size: " << free.size << std::endl;
    }
}

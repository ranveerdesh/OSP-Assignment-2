#include "allocator.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <limits>

// Global variables for memory management
std::list<MemoryAllocation> allocatedList; // List of allocated memory chunks
std::list<MemoryAllocation> freeList;      // List of free memory chunks
std::string allocationStrategy = "firstfit"; // Default allocation strategy

// Set the current memory allocation strategy
void setStrategy(const std::string& strategy) {
    allocationStrategy = strategy; // Update the strategy
}

// Initialize the free list with predefined memory sizes
void initializeFreeList() {
    // Array of fixed memory sizes
    std::size_t memorySizes[] = {32, 64, 128, 256, 512};
    
    for (const auto& size : memorySizes) {
        void* memorySpace = sbrk(size); // Request memory from the operating system
        if (memorySpace == (void*) -1) {
            std::cerr << "Error: Memory allocation failed with sbrk" << std::endl;
            exit(EXIT_FAILURE); // Exit if allocation fails
        }
        freeList.push_back({size, 0, memorySpace}); // Add to free list
    }
}

// Get the appropriate partition size for the requested size
std::size_t getSuitablePartitionSize(std::size_t requestedSize) {
    // Determine the nearest fixed partition size
    if (requestedSize <= 32) return 32;
    if (requestedSize <= 64) return 64;
    if (requestedSize <= 128) return 128;
    if (requestedSize <= 256) return 256;
    return 512; // Return the largest partition size
}

// Allocate memory based on the specified chunk size
void* alloc(std::size_t requestedSize) {
    std::size_t suitableSize = getSuitablePartitionSize(requestedSize); // Get a suitable partition size

    // Determine the iterator for finding the appropriate chunk
    auto chunkIterator = freeList.end();
    
    // Choose allocation strategy
    if (allocationStrategy == "firstfit") {
        // First-Fit: Locate the first available chunk that fits
        chunkIterator = std::find_if(freeList.begin(), freeList.end(),
            [suitableSize](const MemoryAllocation& free) {
                return free.totalSize >= suitableSize; // Check for sufficient size
            });
    } else if (allocationStrategy == "bestfit") {
        // Best-Fit: Locate the chunk with the closest fit
        std::size_t minimalDifference = std::numeric_limits<std::size_t>::max();
        for (auto iter = freeList.begin(); iter != freeList.end(); ++iter) {
            if (iter->totalSize >= suitableSize && (iter->totalSize - suitableSize) < minimalDifference) {
                minimalDifference = iter->totalSize - suitableSize;
                chunkIterator = iter; // Update the iterator to the best fit
            }
        }
    }

    if (chunkIterator != freeList.end()) { // If a suitable chunk is found
        // Allocate memory from the chosen chunk
        MemoryAllocation allocation = *chunkIterator;
        allocation.usedSize = suitableSize; // Update used size
        freeList.erase(chunkIterator); // Remove from free list
        allocatedList.push_back(allocation); // Add to allocated list
        return allocation.memoryAddress; // Return the address of allocated memory
    }

    // No suitable chunk found, extend the heap
    void* newMemorySpace = sbrk(suitableSize); // Request more memory
    if (newMemorySpace == (void*) -1) {
        std::cerr << "Error: Memory allocation failed with sbrk" << std::endl;
        return nullptr; // Return null if allocation fails
    }

    // Add the newly allocated memory to the allocated list
    allocatedList.push_back({suitableSize, suitableSize, newMemorySpace});
    return newMemorySpace; // Return the address of the new memory
}

// Deallocate a previously allocated memory chunk
void dealloc(void* chunk) {
    // Search for the memory chunk in the allocated list
    auto allocationIterator = std::find_if(allocatedList.begin(), allocatedList.end(),
        [chunk](const MemoryAllocation& allocation) {
            return allocation.memoryAddress == chunk; // Match based on memory address
        });

    // Check if the chunk exists in the allocated list
    if (allocationIterator != allocatedList.end()) {
        MemoryAllocation allocation = *allocationIterator; // Get the allocation
        allocatedList.erase(allocationIterator); // Remove it from allocated list
        freeList.push_back(allocation); // Add it to the free list
    } else {
        std::cerr << "Error: Attempted to deallocate a non-existent memory chunk" << std::endl;
        exit(EXIT_FAILURE); // Exit if trying to deallocate non-existent memory
    }
}

// Print the state of allocated and free memory lists
void printMemoryLists() {
    std::cout << "Current Allocated Memory Chunks:" << std::endl;
    for (const auto& allocation : allocatedList) {
        std::cout << "Memory Address: " << allocation.memoryAddress
                  << ", Total Size: " << allocation.totalSize
                  << ", Used Size: " << allocation.usedSize << std::endl;
    }

    std::cout << "Available Free Memory Chunks:" << std::endl;
    for (const auto& free : freeList) {
        std::cout << "Memory Address: " << free.memoryAddress
                  << ", Size: " << free.totalSize << std::endl;
    }
}

#include "allocator.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <limits>

// Global variables for tracking memory allocations
std::list<MemoryAllocation> allocatedList;
std::list<MemoryAllocation> freeList;
std::string currentStrategy = "firstfit"; // Default allocation strategy

void setStrategy(const std::string& strategy) {
    currentStrategy = strategy; // Update the current strategy
}

void initializeFreeList() {
    // Initialize the free list with predefined memory chunk sizes
    std::size_t sizes[] = {32, 64, 128, 256, 512}; // Fixed partition sizes
    for (auto size : sizes) {
        void* memoryBlock = sbrk(size); // Allocate memory from the heap
        if (memoryBlock == (void*) -1) {
            std::cerr << "Error: Unable to allocate memory using sbrk" << std::endl;
            exit(1); // Exit if memory allocation fails
        }
        // Add the allocated block to the free list
        freeList.push_back({size, 0, memoryBlock});
    }
}

std::size_t getPartitionSize(std::size_t requestedSize) {
    // Round up the requested size to the nearest fixed partition size
    if (requestedSize <= 32) return 32;
    else if (requestedSize <= 64) return 64;
    else if (requestedSize <= 128) return 128;
    else if (requestedSize <= 256) return 256;
    else return 512;
}

void* alloc(std::size_t chunk_size) {
    std::size_t partitionSize = getPartitionSize(chunk_size); // Get the closest partition size

    // Iterator for the free list
    auto freeChunkIt = freeList.end();

    // Determine allocation based on the selected strategy
    if (currentStrategy == "firstfit") {
        // First-Fit: Find the first chunk that fits the requested size
        freeChunkIt = std::find_if(freeList.begin(), freeList.end(), [partitionSize](const MemoryAllocation& freeChunk) {
            return freeChunk.totalSize >= partitionSize; // Check if chunk is large enough
        });
    } else if (currentStrategy == "bestfit") {
        // Best-Fit: Find the smallest chunk that fits the requested size
        std::size_t minDifference = std::numeric_limits<std::size_t>::max();
        for (auto it = freeList.begin(); it != freeList.end(); ++it) {
            if (it->totalSize >= partitionSize && (it->totalSize - partitionSize) < minDifference) {
                minDifference = it->totalSize - partitionSize; // Update to the best fit
                freeChunkIt = it;
            }
        }
    }

    if (freeChunkIt != freeList.end()) {
        // Allocate memory from the selected chunk
        MemoryAllocation selectedChunk = *freeChunkIt; // Copy the chunk to be allocated
        selectedChunk.usedSize = partitionSize; // Update the used size
        freeList.erase(freeChunkIt); // Remove the chunk from the free list
        allocatedList.push_back(selectedChunk); // Add the chunk to the allocated list
        return selectedChunk.memoryAddress; // Return the address of the allocated memory
    }

    // No suitable chunk found; request more memory from the OS
    void* newMemoryBlock = sbrk(partitionSize);
    if (newMemoryBlock == (void*) -1) {
        std::cerr << "Error: Unable to allocate memory using sbrk" << std::endl;
        return nullptr; // Return nullptr if allocation fails
    }

    // Add the newly allocated chunk to the allocated list
    allocatedList.push_back({partitionSize, partitionSize, newMemoryBlock});
    return newMemoryBlock; // Return the address of the newly allocated memory
}

void dealloc(void* chunk) {
    // Find the chunk in the allocated list to deallocate
    auto it = std::find_if(allocatedList.begin(), allocatedList.end(), [chunk](const MemoryAllocation& allocChunk) {
        return allocChunk.memoryAddress == chunk; // Check if chunk matches the address
    });

    if (it != allocatedList.end()) {
        MemoryAllocation deallocChunk = *it; // Copy the chunk to be deallocated
        allocatedList.erase(it); // Remove the chunk from the allocated list
        freeList.push_back(deallocChunk); // Add the chunk back to the free list
    } else {
        std::cerr << "Error: Attempting to deallocate non-existent chunk" << std::endl;
        exit(1); // Exit if trying to deallocate a chunk not found
    }
}

void printMemoryLists() {
    std::cout << "Current Allocated Memory Chunks:" << std::endl;
    for (const auto& allocChunk : allocatedList) {
        std::cout << "Memory Address: " << allocChunk.memoryAddress
                  << ", Total Size: " << allocChunk.totalSize
                  << ", Used Size: " << allocChunk.usedSize << std::endl;
    }

    std::cout << "Available Free Memory Chunks:" << std::endl;
    for (const auto& freeChunk : freeList) {
        std::cout << "Memory Address: " << freeChunk.memoryAddress
                  << ", Size: " << freeChunk.totalSize << std::endl;
    }
}

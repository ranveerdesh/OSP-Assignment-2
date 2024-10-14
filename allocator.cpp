#include "allocator.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <limits>

// Global variables
std::list<Allocation> allocatedList;
std::list<Allocation> freeList;
std::string currentStrategy = "firstfit"; // Default strategy is first fit

void setStrategy(const std::string& strategy) {
    currentStrategy = strategy;
}

void initializeFreeList() {
    // Initialize the free list with predefined sizes
    std::size_t sizes[] = {32, 64, 128, 256, 512};
    for (auto size : sizes) {
        void* space = sbrk(size);
        if (space == (void*) -1) {
            std::cerr << "Error: Unable to allocate memory using sbrk" << std::endl;
            exit(1);
        }
        freeList.push_back({size, 0, space});
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
    std::size_t partitionSize = getPartitionSize(chunk_size);

    // Allocation based on the current strategy
    auto it = freeList.end();
    if (currentStrategy == "firstfit") {
        // First-Fit: Find the first chunk that fits
        it = std::find_if(freeList.begin(), freeList.end(), [partitionSize](const Allocation& free) {
            return free.totalSize >= partitionSize;
        });
    } else if (currentStrategy == "bestfit") {
        // Best-Fit: Find the smallest chunk that fits
        std::size_t minDiff = std::numeric_limits<std::size_t>::max();
        for (auto iter = freeList.begin(); iter != freeList.end(); ++iter) {
            if (iter->totalSize >= partitionSize && (iter->totalSize - partitionSize) < minDiff) {
                minDiff = iter->totalSize - partitionSize;
                it = iter;
            }
        }
    }

    if (it != freeList.end()) {
        // Allocate memory from the selected chunk
        Allocation alloc = *it;
        alloc.usedSize = partitionSize;
        freeList.erase(it);
        allocatedList.push_back(alloc);
        return alloc.space;
    }

    // No suitable chunk found, grow the heap
    void* space = sbrk(partitionSize);
    if (space == (void*) -1) {
        std::cerr << "Error: Unable to allocate memory using sbrk" << std::endl;
        return nullptr;
    }

    // Add to the allocated list
    allocatedList.push_back({partitionSize, partitionSize, space});
    return space;
}

void dealloc(void* chunk) {
    auto it = std::find_if(allocatedList.begin(), allocatedList.end(), [chunk](const Allocation& alloc) {
        return alloc.space == chunk;
    });

    if (it != allocatedList.end()) {
        Allocation alloc = *it;
        allocatedList.erase(it);
        freeList.push_back(alloc);
    } else {
        std::cerr << "Error: Attempting to deallocate non-existent chunk" << std::endl;
        exit(1);
    }
}

void printMemoryLists() {
    std::cout << "Allocated List:" << std::endl;
    for (const auto& alloc : allocatedList) {
        std::cout << "Address: " << alloc.space
                  << ", Total Size: " << alloc.totalSize
                  << ", Used Size: " << alloc.usedSize << std::endl;
    }

    std::cout << "Free List:" << std::endl;
    for (const auto& free : freeList) {
        std::cout << "Address: " << free.space
                  << ", Size: " << free.totalSize << std::endl;
    }
}

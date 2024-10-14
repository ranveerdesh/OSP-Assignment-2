#include "allocator.h"
#include <cstdlib>
#include <cstring>
#include <unistd.h>

enum AllocationStrategy {
    FIRST_FIT,
    BEST_FIT
};

AllocationStrategy strategy = FIRST_FIT;

// Initialize free list with predefined sizes
void initializeFreeList() {
    freeList.push_back({32, nullptr});
    freeList.push_back({64, nullptr});
    freeList.push_back({128, nullptr});
    freeList.push_back({256, nullptr});
    freeList.push_back({512, nullptr});
}

// Function to allocate memory
void *alloc(std::size_t chunk_size) {
    // Find a suitable chunk in the free list
    for (auto it = freeList.begin(); it != freeList.end(); ++it) {
        if (it->size >= chunk_size) {
            // Allocate memory
            void *chunk = sbrk(it->size);
            if (chunk == (void *)-1) {
                std::cerr << "Memory allocation failed!" << std::endl;
                return nullptr;
            }

            // Create an allocation entry
            Allocation allocation = {it->size, chunk};
            allocatedList.push_back(allocation);
            freeList.erase(it); // Remove from free list
            return chunk;
        }
    }

    // No suitable chunk found, request more memory
    void *chunk = sbrk(chunk_size);
    if (chunk == (void *)-1) {
        std::cerr << "Memory allocation failed!" << std::endl;
        return nullptr;
    }

    // Add new allocation to the allocated list
    Allocation allocation = {chunk_size, chunk};
    allocatedList.push_back(allocation);
    return chunk;
}

// Function to deallocate memory
void dealloc(void *chunk) {
    // Find the allocated chunk in the allocated list
    auto it = std::find_if(allocatedList.begin(), allocatedList.end(), [chunk](const Allocation& alloc) {
        return alloc.space == chunk;
    });

    if (it == allocatedList.end()) {
        std::cerr << "Error: Attempting to deallocate a chunk that was not allocated!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Move the allocation to the free list
    freeList.push_back(*it);
    allocatedList.erase(it);
}

// Function to print memory lists
void printMemoryLists() {
    std::cout << "Allocated List:" << std::endl;
    for (const auto &alloc : allocatedList) {
        std::cout << "Address: " << alloc.space << ", Size: " << alloc.size << std::endl;
    }

    std::cout << "Free List:" << std::endl;
    for (const auto &free : freeList) {
        std::cout << "Address: " << free.space << ", Size: " << free.size << std::endl;
    }
}

// Set the allocation strategy
void setStrategy(const std::string& strat) {
    if (strat == "firstfit") {
        strategy = FIRST_FIT;
    } else if (strat == "bestfit") {
        strategy = BEST_FIT;
    } else {
        std::cerr << "Invalid strategy: " << strat << std::endl;
    }
}

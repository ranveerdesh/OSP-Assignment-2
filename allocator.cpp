#include "allocator.h"
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <algorithm>

enum AllocationStrategy {
    FIRST_FIT,
    BEST_FIT
};

AllocationStrategy strategy = FIRST_FIT;

// Define global variables here
std::vector<Allocation> allocatedList; // Define here
std::vector<Allocation> freeList; // Define here

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
    for (auto it = freeList.begin(); it != freeList.end(); ++it) {
        if (it->size >= chunk_size) {
            void *chunk = sbrk(it->size);
            if (chunk == (void *)-1) {
                std::cerr << "Memory allocation failed!" << std::endl;
                return nullptr;
            }

            Allocation allocation = {it->size, chunk};
            allocatedList.push_back(allocation);
            freeList.erase(it);
            return chunk;
        }
    }

    void *chunk = sbrk(chunk_size);
    if (chunk == (void *)-1) {
        std::cerr << "Memory allocation failed!" << std::endl;
        return nullptr;
    }

    Allocation allocation = {chunk_size, chunk};
    allocatedList.push_back(allocation);
    return chunk;
}

// Function to deallocate memory
void dealloc(void *chunk) {
    auto it = std::find_if(allocatedList.begin(), allocatedList.end(), [chunk](const Allocation& alloc) {
        return alloc.space == chunk;
    });

    if (it == allocatedList.end()) {
        std::cerr << "Error: Attempting to deallocate a chunk that was not allocated!" << std::endl;
        exit(EXIT_FAILURE);
    }

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

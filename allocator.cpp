#include "allocator.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <unistd.h>


// Global variables
std::vector<Allocation> allocatedList;
std::vector<Allocation> freeList;

void setStrategy(const std::string& strategy) {
    // You can implement logic for different strategies if needed.
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
        freeList.push_back({size, space});
    }
}

void* alloc(std::size_t chunk_size) {
    for (auto it = freeList.begin(); it != freeList.end(); ++it) {
        if (it->size >= chunk_size) {
            // Use the first fit strategy
            Allocation alloc = *it;
            freeList.erase(it);
            allocatedList.push_back(alloc);
            return alloc.space;
        }
    }

    // If no suitable free chunk is found, request more memory
    void* space = sbrk(chunk_size);
    if (space == (void*) -1) {
        std::cerr << "Error: Unable to allocate memory using sbrk" << std::endl;
        return nullptr;
    }
    
    allocatedList.push_back({chunk_size, space});
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
        std::cout << "Address: " << alloc.space << ", Size: " << alloc.size << std::endl;
    }
    
    std::cout << "Free List:" << std::endl;
    for (const auto& free : freeList) {
        std::cout << "Address: " << free.space << ", Size: " << free.size << std::endl;
    }
}

#include <iostream>
#include <fstream>
#include <string>
#include "allocator.h"

int main(int argc, char *argv[]) {
    // Ensure the correct number of command-line arguments is provided
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <allocation_strategy> <datafile>" << std::endl;
        return 1;
    }

    // Retrieve command-line arguments for strategy and data file
    std::string allocationStrategy = argv[1];
    std::string dataFileName = argv[2];

    // Set the memory allocation strategy based on the provided argument
    setStrategy(allocationStrategy);

    // Initialize the free memory list
    initializeFreeList();

    // Open the specified data file for reading
    std::ifstream dataFile(dataFileName);
    if (!dataFile.is_open()) {
        std::cerr << "Error: Unable to open data file: " << dataFileName << std::endl;
        return 1;
    }

    std::string operation;
    std::size_t size;

    // Process commands from the data file
    while (dataFile >> operation >> size) {
        if (operation == "alloc") {
            // Attempt to allocate memory of the specified size
            void* allocatedChunk = alloc(size);
            if (allocatedChunk) {
                std::cout << "Allocated " << size << " bytes at address " << allocatedChunk << std::endl;
            } else {
                std::cerr << "Allocation failed for " << size << " bytes." << std::endl;
            }
        } else if (operation == "dealloc") {
            // Deallocate the last allocated chunk, if it exists
            if (!allocatedList.empty()) {
                void* lastAllocatedChunk = allocatedList.back().memoryAddress; // Get the last allocated chunk
                dealloc(lastAllocatedChunk);
                std::cout << "Deallocated chunk at address " << lastAllocatedChunk << std::endl;
            } else {
                std::cerr << "No allocated chunks to deallocate." << std::endl;
            }
        }
    }

    // Close the data file after processing
    dataFile.close();

    // Print the current memory lists for debugging
    printMemoryLists();

    return 0;
}

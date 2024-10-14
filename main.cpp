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
    while (dataFile >> operation) {
        std::cout << "Read operation: " << operation << std::endl;  // Debug output

        // Check for "alloc" operation
        if (operation == "alloc") {
            // Read the size for allocation
            if (dataFile >> size) {
                std::cout << "Allocating " << size << " bytes." << std::endl;  // Debug output
                // Attempt to allocate memory of the specified size
                void* allocatedChunk = alloc(size);
                if (allocatedChunk) {
                    std::cout << "Allocated " << size << " bytes at address " << allocatedChunk << std::endl;
                } else {
                    std::cerr << "Allocation failed for " << size << " bytes." << std::endl;
                }
            } else {
                std::cerr << "Error reading size for allocation. Skipping line." << std::endl;
                dataFile.clear(); // Clear the fail state
                std::string skipLine;
                std::getline(dataFile, skipLine); // Skip to the end of the line
            }
        } 
        // Check for "dealloc" operation
        else if (operation == "dealloc") {
            std::cout << "Deallocating last allocated chunk." << std::endl;  // Debug output
            // Deallocate the last allocated chunk, if it exists
            if (!allocatedList.empty()) {
                void* lastAllocatedChunk = allocatedList.back().memoryAddress; // Get the last allocated chunk
                dealloc(lastAllocatedChunk);
                std::cout << "Deallocated chunk at address " << lastAllocatedChunk << std::endl;
            } else {
                std::cerr << "No allocated chunks to deallocate." << std::endl;
            }
        } 
        // Handle unknown operations
        else {
            std::cerr << "Unknown operation: " << operation << std::endl;
            // Clear the fail state and skip to the end of the line
            dataFile.clear();
            std::string skipLine;
            std::getline(dataFile, skipLine);
        }
    }

    // Close the data file after processing
    dataFile.close();

    // Print the current memory lists for debugging
    printMemoryLists();

    return 0;
}

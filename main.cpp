#include <iostream>
#include <fstream>
#include <string>
#include "allocator.h"

// Entry point of the memory allocator program
int main(int argc, char* argv[]) {
    // Validate the number of command-line arguments
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <allocation_strategy> <input_file>" << std::endl;
        return EXIT_FAILURE; // Return a failure status
    }

    // Extract command-line arguments for allocation strategy and input file
    std::string allocationStrategy = argv[1];
    std::string inputFileName = argv[2];

    // Set the allocation strategy based on user input
    setStrategy(allocationStrategy);

    // Initialize the list of free memory chunks
    initializeFreeList();

    // Open the specified input data file
    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open input file: " << inputFileName << std::endl;
        return EXIT_FAILURE; // Return a failure status
    }

    std::string command; // Command for allocation or deallocation
    std::size_t size;    // Size for memory allocation

    // Process commands from the input file
    while (inputFile >> command >> size) {
        if (command == "alloc") { // Check for allocation command
            void* allocatedMemory = alloc(size);
            if (allocatedMemory) {
                std::cout << "Allocated " << size << " bytes at " << allocatedMemory << std::endl;
            }
        } else if (command == "dealloc") { // Check for deallocation command
            if (!allocatedList.empty()) {
                void* lastAllocatedChunk = allocatedList.back().memoryAddress; // Corrected: Get the last allocated chunk
                dealloc(lastAllocatedChunk); // Deallocate the chunk
                std::cout << "Deallocated chunk at " << lastAllocatedChunk << std::endl;
            }
        }
    }

    inputFile.close(); // Close the input file

    // Print the current state of the allocated and free memory lists
    printMemoryLists();

    return EXIT_SUCCESS; // Return a success status
}

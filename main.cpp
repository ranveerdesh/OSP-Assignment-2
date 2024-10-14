#include "allocator.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <datafile>" << std::endl;
        return 1;
    }

    initMemoryManagement(); // Initialize the memory management system

    // Read the data file and process allocations and deallocations
    std::ifstream dataFile(argv[1]);
    if (!dataFile.is_open()) {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }

    std::string command;
    std::vector<void*> allocatedChunks; // To track allocated chunks for deallocation
    while (dataFile >> command) {
        std::size_t size;
        dataFile >> size;

        if (command == "alloc") {
            void *chunk = alloc(size);
            if (chunk) {
                allocatedChunks.push_back(chunk); // Keep track of allocated chunks
            }
        } else if (command == "dealloc") {
            if (!allocatedChunks.empty()) {
                void *chunk = allocatedChunks.back();
                dealloc(chunk); // Deallocate the last allocated chunk
                allocatedChunks.pop_back(); // Remove from tracking list
            } else {
                std::cerr << "Error: No allocated chunks to deallocate!" << std::endl;
            }
        }
    }

    // Print the memory lists
    printMemoryLists();
    return 0;
}

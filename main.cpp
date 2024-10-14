#include <iostream>
#include <fstream>
#include <string>
#include "allocator.h"

int main(int argc, char *argv[]) {
    // Check the number of command-line arguments
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <strategy> <datafile>" << std::endl;
        return 1;
    }

    // Parse command-line arguments
    std::string strategy = argv[1];
    std::string datafile = argv[2];

    // Set the allocation strategy
    setStrategy(strategy);

    // Initialize the free list
    initializeFreeList();

    // Open the data file
    std::ifstream file(datafile);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open data file: " << datafile << std::endl;
        return 1;
    }

    std::string operation;
    std::size_t size;

    // Process the data file
    while (file >> operation >> size) {
        if (operation == "alloc") {
            void* chunk = alloc(size);
            if (chunk) {
                std::cout << "Allocated " << size << " bytes at " << chunk << std::endl;
            }
        } else if (operation == "dealloc") {
            if (!allocatedList.empty()) {
                void* lastChunk = allocatedList.back().space; // Get the last allocated chunk
                dealloc(lastChunk);
                std::cout << "Deallocated chunk at " << lastChunk << std::endl;
            }
        }
    }

    file.close();

    // Print memory lists
    printMemoryLists();

    return 0;
}

#include "allocator.h"
#include <fstream>
#include <iostream>
#include <string>

void processAllocations(const std::string &filename);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <strategy> <datafile>" << std::endl;
        return EXIT_FAILURE;
    }

    setStrategy(argv[1]);
    processAllocations(argv[2]);
    printMemoryLists();

    return EXIT_SUCCESS;
}

void processAllocations(const std::string &filename) {
    std::ifstream infile(filename);
    std::string line;

    if (!infile) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    while (std::getline(infile, line)) {
        std::size_t size;
        if (line.substr(0, 5) == "alloc") {
            size = std::stoul(line.substr(6));
            alloc(size);
        } else if (line.substr(0, 6) == "dealloc") {
            if (!allocatedList.empty()) {
                void *chunk = allocatedList.back().space; // Get the last allocated chunk
                dealloc(chunk);
            }
        }
    }

    infile.close();
}

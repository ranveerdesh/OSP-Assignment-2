// main.cpp
#include "allocator.h"

int main() {
    initMemoryManagement(); // Initialize memory management

    // Example allocations and deallocations
    void *ptr1 = alloc(50); // Should allocate a 64-byte chunk
    void *ptr2 = alloc(30); // Should allocate a 32-byte chunk

    printMemoryLists(); // Print current memory lists

    dealloc(ptr1); // Deallocate the first chunk
    dealloc(ptr2); // Deallocate the second chunk

    printMemoryLists(); // Print memory lists after deallocation

    return 0;
}

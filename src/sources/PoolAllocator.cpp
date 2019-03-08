#include <PoolAllocator.hpp>

using namespace puggo;

PoolAllocator::PoolAllocator(size_t objectSize, unsigned char objectAlignment, size_t size, void* mem):
    Allocator(size, mem), objectSize(objectSize), objectAlignment(objectAlignment) {
    assert(objectSize >= sizeof(void*));

    // Calculate adjustment needed to keep object correctly aligned
    unsigned char adjustment = alignForwardAdjustment(mem, objectAlignment);
    freeList = (void**)((unsigned char*)mem + adjustment);
    size_t numObjects = (size - adjustment) / objectSize;
    void** p = freeList;

    // Initialize free blocks list
    for (size_t i = 0; i < numObjects - 1; i++) {
        *p = (void*)((unsigned char*)p + objectSize);
        p = (void**) *p;
    }

    *p = nullptr;
}

PoolAllocator::PoolAllocator(PoolAllocator&& allocator) noexcept:
    Allocator(move(allocator.size), move(allocator.start)) {
    numAllocations = move(allocator.numAllocations);
    usedMemory = move(allocator.usedMemory);
    freeList = move(allocator.freeList);
    objectAlignment = move(allocator.objectAlignment);
    objectSize = move(allocator.objectSize);
}

PoolAllocator::~PoolAllocator(void) {
    freeList = nullptr;
}

void* PoolAllocator::allocate(const size_t& size, const unsigned char& alignment) {
    assert(size == objectSize && alignment == objectAlignment);
    if (freeList == nullptr) {
        return nullptr;
    }

    void* p = freeList;
    freeList = (void**)(*freeList);
    usedMemory += size;
    numAllocations++;
    return p;
}

void PoolAllocator::deallocate(void* p) {
    *((void**)p) = freeList;
    freeList = (void**)p;
    usedMemory -= objectSize;
    numAllocations--;
}
#include <LinearAllocator.hpp>

using namespace puggo;

LinearAllocator::LinearAllocator(size_t size, void* start) :
    Allocator(size, start),
    currentPos(start) {
    assert(size > 0);
}

LinearAllocator::LinearAllocator(LinearAllocator&& allocator) noexcept:
    Allocator(move(allocator.size), move(allocator.start)) {
    numAllocations = move(allocator.numAllocations);
    usedMemory = move(allocator.usedMemory);
    currentPos = move(allocator.currentPos);
}

LinearAllocator::~LinearAllocator(void) {
    currentPos = nullptr;
}

void * LinearAllocator::allocate(const size_t& size, const unsigned char& alignment) {
    assert(size != 0 && alignment != 0);
    unsigned char adjustment = alignForwardAdjustment(currentPos, alignment);

    if (usedMemory + adjustment + size > this->size) {
        return nullptr;
    }

    unsigned char* alignedAddress = (unsigned char*)currentPos + adjustment;
    currentPos = (void*)(alignedAddress + size);
    usedMemory += size + adjustment;
    numAllocations++;

    return (void*)alignedAddress;
}

void LinearAllocator::deallocate(void* p) {
    assert(false && "Use clear() instead");
}

void LinearAllocator::clear(void) {
    numAllocations = 0;
    usedMemory = 0;
    currentPos = start;
}
#include <StackAllocator.hpp>

using namespace puggo;

StackAllocator::StackAllocator(size_t size, void* start):
    Allocator(size, start),
    currentPos(start) {
    assert(size > 0);

#if _DEBUG
    prevPosition = nullptr;
#endif
}

StackAllocator::StackAllocator(StackAllocator&& allocator) noexcept:
    Allocator(move(allocator.size), move(allocator.start)) {
    numAllocations = move(allocator.numAllocations);
    usedMemory = move(allocator.usedMemory);
    currentPos = move(allocator.currentPos);
#if _DEBUG
    prevPosition = move(allocator.prevPosition);
#endif
}

StackAllocator::~StackAllocator(void) {
#if _DEBUG
    prevPosition = nullptr;
#endif

    currentPos = nullptr;
}

void* StackAllocator::allocate(const size_t& size, const unsigned char& alignment) {
    assert(size != 0 && alignment != 0);
    unsigned char adjustment = alignForwardAdjustmentWithHeader(currentPos, alignment, sizeof(AllocationHeader));

    if (usedMemory + adjustment + size > this->size) {
        return nullptr;
    }

    void* alignedAddress = (void*)((unsigned char*)currentPos + adjustment);

    // Add Allocation Header
    AllocationHeader* header = (AllocationHeader*)((unsigned char*)alignedAddress - sizeof(AllocationHeader));
    header->adjustment = adjustment;

#if _DEBUG
    header->prevAddress = prevPosition;
    prevPosition = alignedAddress;
#endif

    currentPos = (void*)((unsigned char*)alignedAddress + size);
    usedMemory += size + adjustment;
    numAllocations++;

    return alignedAddress;
}

void StackAllocator::deallocate(void* p) {
#if _DEBUG 
    assert(p == prevPosition);
#endif

    // Access the AllocationHeader in the bytes before p
    AllocationHeader* header = (AllocationHeader*)((unsigned char*)p - sizeof(AllocationHeader));
    usedMemory -= (unsigned char*)currentPos - (unsigned char*)p + header->adjustment;
    currentPos = (unsigned char*)p - header->adjustment;

#if _DEBUG
    prevPosition = header->prevAddress;
#endif

    numAllocations--;
}
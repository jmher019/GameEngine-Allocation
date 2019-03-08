#include <Allocator.hpp>

using namespace puggo;

Allocator::Allocator(const size_t& size, void* start) {
    this->start = start;
    this->size = size;
    this->usedMemory = 0;
    this->numAllocations = 0;
}

Allocator::~Allocator(void) {
    assert(numAllocations == 0 && usedMemory == 0);
    start = nullptr;
    size = 0;
}

void* Allocator::getStart(void) const {
    return start;
}

size_t Allocator::getSize(void) const {
    return size;
}

size_t Allocator::getUsedMemory(void) const {
    return usedMemory;
}

size_t Allocator::getNumAllocations(void) const {
    return numAllocations;
}
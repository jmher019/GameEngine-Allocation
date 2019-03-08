#include <ProxyAllocator.hpp>

using namespace puggo;

ProxyAllocator::ProxyAllocator(Allocator& allocator):
    Allocator(allocator.getSize(), allocator.getStart()),
    allocator(allocator) {
}

ProxyAllocator::~ProxyAllocator(void) {}

void* ProxyAllocator::allocate(const size_t& size, const unsigned char& alignment) {
    assert(size != 0);
    numAllocations++;
    size_t mem = allocator.getUsedMemory();

    void* p = allocator.allocate(size, alignment);
    usedMemory += allocator.getUsedMemory() - mem;
    return p;
}

void ProxyAllocator::deallocate(void* p) {
    numAllocations--;
    size_t mem = allocator.getUsedMemory();
    allocator.deallocate(p);
    usedMemory -= mem - allocator.getUsedMemory();
}
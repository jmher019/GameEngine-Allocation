#include <ProxyAllocator.hpp>

using namespace puggo;

ProxyAllocator::ProxyAllocator(Allocator& allocator):
    Allocator(allocator.getSize(), allocator.getStart()),
    allocator(allocator) {
}

ProxyAllocator::~ProxyAllocator(void) {}

Result<void*, nullptr_t> ProxyAllocator::allocate(const size_t& size, const unsigned char& alignment) {
	if (size == 0) {
		return Result<void*, nullptr_t>::error(nullptr);
	}


    numAllocations++;
    size_t mem = allocator.getUsedMemory();

	Result<void*, nullptr_t> result = Result<void*, nullptr_t>::error(nullptr);
	allocator
		.allocate(size, alignment)
		.map([&result](void* p) {
			result = Result<void*, nullptr_t>::ok(p);
		});

	usedMemory += allocator.getUsedMemory() - mem;
    return result;
}

void ProxyAllocator::deallocate(void* p) {
    numAllocations--;
    size_t mem = allocator.getUsedMemory();
    allocator.deallocate(p);
    usedMemory -= mem - allocator.getUsedMemory();
}
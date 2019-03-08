#ifndef POOL_ALLOCATOR_HPP
#define POOL_ALLOCATOR_HPP

#include <iostream>

#include <Allocator.hpp>

using namespace std;

namespace puggo {
    class PoolAllocator: public Allocator {
    public:
        PoolAllocator(size_t objectSize, unsigned char objectAlignment, size_t size, void* mem);
        PoolAllocator(const PoolAllocator&) = delete;
        PoolAllocator(PoolAllocator&& allocator) noexcept;
        ~PoolAllocator(void);

        PoolAllocator& operator=(const PoolAllocator&) = delete;
        PoolAllocator& operator=(PoolAllocator&&) = delete;

        void* allocate(const size_t& size, const unsigned char& alignment) override;
        void deallocate(void* p) override;

    private:
        size_t objectSize;
        unsigned char objectAlignment;
        void** freeList;
    };
}

#endif // !POOL_ALLOCATOR_HPP

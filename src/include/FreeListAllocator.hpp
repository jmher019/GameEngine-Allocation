#ifndef PUGGO_FREE_LIST_ALLOCATOR
#define PUGGO_FREE_LIST_ALLOCATOR

#include <iostream>

#include <Allocator.hpp>

using namespace std;

namespace puggo {
    class FreeListAllocator : public Allocator {
    public:
        FreeListAllocator(size_t size, void* start);
        FreeListAllocator(const FreeListAllocator&) = delete;
        FreeListAllocator(FreeListAllocator&& allocator) noexcept;
        ~FreeListAllocator(void);

        FreeListAllocator& operator=(const FreeListAllocator&) = delete;
        FreeListAllocator& operator=(FreeListAllocator&&) = delete;

        void* allocate(const size_t& size, const unsigned char& alignment) override;
        void deallocate(void* p) override;

    private:
        struct AllocationHeader {
            size_t size;
            unsigned char adjustment;
        };

        struct FreeBlock {
            size_t size;
            FreeBlock* next;
        };

        FreeBlock* freeBlocks;
    };
}

#endif // !PUGGO_FREE_LIST_ALLOCATOR

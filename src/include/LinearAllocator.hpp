#ifndef PUGGO_LINEAR_ALLOCATOR
#define PUGGO_LINEAR_ALLOCATOR

#include <iostream>

#include <Allocator.hpp>

using namespace std;

namespace puggo {
    class LinearAllocator : public Allocator {
    public:
        LinearAllocator(size_t size, void* start);
        LinearAllocator(const LinearAllocator&) = delete;
        LinearAllocator(LinearAllocator&& allocator) noexcept;
        ~LinearAllocator(void);

        LinearAllocator& operator=(const LinearAllocator&) = delete;
        LinearAllocator& operator=(LinearAllocator&&) = delete;
        Result<void*, nullptr_t> allocate(const size_t& size, const unsigned char& alignment) override;
        void deallocate(void* p) override;
        void clear(void);

    private:
        void* currentPos;
    };
}

#endif // !PUGGO_LINEAR_ALLOCATOR

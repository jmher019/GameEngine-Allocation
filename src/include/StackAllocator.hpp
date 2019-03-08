#ifndef PUGGO_STACK_ALLOCATOR_HPP
#define PUGGO_STACK_ALLOCATOR_HPP

#include <iostream>

#include <Allocator.hpp>

using namespace std;

namespace puggo {
    class StackAllocator : public Allocator {
    public:
        StackAllocator(size_t size, void* start);
        StackAllocator(const StackAllocator&) = delete;
        StackAllocator(StackAllocator&& allocator) noexcept;
        ~StackAllocator(void);

        StackAllocator& operator=(const StackAllocator&) = delete;
        StackAllocator& operator=(StackAllocator&&) = delete;

        void* allocate(const size_t& size, const unsigned char& alignment) override;
        void deallocate(void* p) override;

    private:
        struct AllocationHeader
        {
#if _DEBUG 
            void* prevAddress;
#endif 

            unsigned char adjustment;
        };

#if _DEBUG 
        void* prevPosition;
#endif 

        void* currentPos;
    };
}

#endif // !PUGGO_STACK_ALLOCATOR_HPP

#ifndef PUGGO_ALLOCATOR_HPP
#define PUGGO_ALLOCATOR_HPP

#include <assert.h>

namespace puggo {
    class Allocator {
    public:
        Allocator(const size_t& size, void* start);

        virtual ~Allocator(void);

        virtual void* allocate(const size_t& size, const unsigned char& alignment = 4) = 0;
        virtual void deallocate(void* p) = 0;
        void* getStart(void) const;
        size_t getSize(void) const;
        size_t getUsedMemory(void) const;
        size_t getNumAllocations(void) const;

    protected:
        void* start;
        size_t size;
        size_t usedMemory;
        size_t numAllocations;
    };

    template <typename T>
    inline T* allocateNew(Allocator& allocator);

    template <typename T>
    inline T* allocateNew(Allocator& allocator, const T& t);

    template <typename T>
    inline void deallocateDelete(Allocator& allocator, T* object);

    template <typename T>
    inline T* allocateArray(Allocator& allocator, size_t length);

    template <typename T>
    inline void deallocateArray(Allocator& allocator, T* array);

    inline unsigned char alignForwardAdjustment(const void* address, const unsigned char& alignment);

    inline unsigned char alignForwardAdjustmentWithHeader(
        const void* address,
        const unsigned char& alignment,
        const unsigned char& headerSize
    );

#include <Allocator.inl>
}

#endif // !PUGGO_ALLOCATOR_HPP

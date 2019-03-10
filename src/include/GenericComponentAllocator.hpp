#ifndef PUGGO_GENERIC_COMPONENT_ALLOCATOR
#define PUGGO_GENERIC_COMPONENT_ALLOCATOR

#include <deque>
#include <memory>
#include <initializer_list>

#include <GenericHandle.hpp>
#include <AllocationSystem.hpp>

namespace puggo {
    template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle>
    class GenericComponentAllocator {
    public:
        GenericComponentAllocator(const GenericComponentAllocator&) = delete;
        GenericComponentAllocator& operator=(const GenericComponentAllocator&) = delete;

        static void init(const uint32_t& genericComponentLimit = static_cast<uint32_t>(pow(2, IndexSize)));
        static void deallocate(void);

        static Handle create(void);
        static T& get(const Handle& handle);
        static void destroy(const Handle& handle);
        static bool isInitialized(void);
        static bool isValidHandle(const Handle& handle);

    private:
        GenericComponentAllocator(const uint32_t& genericComponentLimit);
        GenericComponentAllocator(GenericComponentAllocator&& componentAllocator) noexcept;

        GenericComponentAllocator& operator=(GenericComponentAllocator&& componentAllocator) noexcept;
        Handle createInternal(void);
        T& getInternal(const Handle& handle);
        void destroyInternal(const Handle& handle);
        bool isInitializedInternal(void);

        void freeMemory(void);
        bool isValidHandleInternal(const Handle& handle);

        void* allocatedMemory = nullptr;
        deque<uint32_t> freeIndices;
        uint32_t genericComponentLimit;
        uint32_t minimumFreeIndices = 1024;
        uint32_t generationSize = 0;
        uint32_t* generation = nullptr;
        T* objects = nullptr;

        static GenericComponentAllocator<T, IndexSize, GenerationSize, Handle> componentAllocator;
    };
}

#include <GenericComponentAllocator.inl>

#endif // !PUGGO_GENERIC_COMPONENT_ALLOCATOR

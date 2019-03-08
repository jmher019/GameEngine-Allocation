#ifndef PROXY_ALLOCATOR_HPP
#define PROXY_ALLOCATOR_HPP

#include <Allocator.hpp>

namespace puggo {
    class ProxyAllocator : public Allocator {
    public:
        ProxyAllocator(Allocator& allocator);
        ProxyAllocator(const ProxyAllocator&) = delete;
        ProxyAllocator(ProxyAllocator&&) = delete;
        ~ProxyAllocator(void);

        ProxyAllocator& operator=(const ProxyAllocator&) = delete;
        ProxyAllocator& operator=(ProxyAllocator&&) = delete;

        void* allocate(const size_t& size, const unsigned char& alignment) override;
        void deallocate(void* p) override;

    private:
        Allocator& allocator;
    };
}

#endif // !PROXY_ALLOCATOR_HPP

#ifndef ALLOCATION_SYSTEM_HPP
#define ALLOCATION_SYSTEM_HPP

#include <algorithm>
#include <vector>

#include <LinearAllocator.hpp>

using namespace std;

namespace puggo {
    class AllocationSystem {
    public:
        static constexpr unsigned long long ONE_GIGABYTE = static_cast<unsigned long long>(1) << 30;
        static constexpr unsigned long long TWO_GIGABYTE = static_cast<unsigned long long>(2) << 30;
        static constexpr unsigned long long THREE_GIGABYTE = static_cast<unsigned long long>(3) << 30;
        static constexpr unsigned long long FOUR_GIGABYTE = static_cast<unsigned long long>(4) << 30;
        static constexpr unsigned long long FIVE_GIGABYTE = static_cast<unsigned long long>(5) << 30;
        static constexpr unsigned long long SIX_GIGABYTE = static_cast<unsigned long long>(6) << 30;
        static constexpr unsigned long long SEVEN_GIGABYTE = static_cast<unsigned long long>(7) << 30;
        static constexpr unsigned long long EIGHT_GIGABYTE = static_cast<unsigned long long>(8) << 30;
        static constexpr unsigned long long NINE_GIGABYTE = static_cast<unsigned long long>(9) << 30;
        static constexpr unsigned long long TEN_GIGABYTE = static_cast<unsigned long long>(10) << 30;
        static constexpr unsigned long long ELEVEN_GIGABYTE = static_cast<unsigned long long>(11) << 30;
        static constexpr unsigned long long TWELVE_GIGABYTE = static_cast<unsigned long long>(12) << 30;
        static constexpr unsigned long long THIRTEEN_GIGABYTE = static_cast<unsigned long long>(13) << 30;
        static constexpr unsigned long long FOURTEEN_GIGABYTE = static_cast<unsigned long long>(14) << 30;
        static constexpr unsigned long long FIFTEEN_GIGABYTE = static_cast<unsigned long long>(15) << 30;
        static constexpr unsigned long long SIXTEEN_GIGABYTE = static_cast<unsigned long long>(16) << 30;

        AllocationSystem(const AllocationSystem&) = delete;
        AllocationSystem(AllocationSystem&&) = delete;
        ~AllocationSystem(void);
        AllocationSystem& operator=(const AllocationSystem&) = delete;

#ifdef _WIN64
        static void init(unsigned long long size, size_t sizePerAllocator = static_cast<size_t>(AllocationSystem::TWO_GIGABYTE));
#else
        static void init(unsigned long long size, size_t sizePerAllocator = static_cast<size_t>(AllocationSystem::ONE_GIGABYTE));
#endif // _WIN64
        static void* requestMemoryFromSystem(size_t size);

    private:
        AllocationSystem(void);
        AllocationSystem(unsigned long long size, size_t sizePerAllocator);

        AllocationSystem& operator=(AllocationSystem&& system) noexcept;
        void initMemory(unsigned long long size, size_t sizePerAllocator);
        void freeAllMemory(void);
        void* requestMemory(size_t size);

        vector<LinearAllocator> masterAllocators;
        vector<void*> dataArr;
        static AllocationSystem system;
    };
}

#endif // !ALLOCATION_SYSTEM_HPP

#include <iostream>

#include <LinearAllocator.hpp>
#include <StackAllocator.hpp>
#include <FreeListAllocator.hpp>
#include <PoolAllocator.hpp>
#include <ProxyAllocator.hpp>
#include <AllocationSystem.hpp>
#include <GenericHandle.hpp>
#include <RenderManager.hpp>

using namespace puggo;

struct a {
    unsigned int i;
    float x;
    float y;
    float z;
};

struct b {
    float* data;
};

int main() {
    AllocationSystem::init(AllocationSystem::FOUR_GIGABYTE);

    FreeListAllocator allocator1(AllocationSystem::TWO_GIGABYTE, AllocationSystem::requestMemoryFromSystem(AllocationSystem::TWO_GIGABYTE));
    PoolAllocator allocator2(sizeof(a), alignof(a), AllocationSystem::TWO_GIGABYTE, AllocationSystem::requestMemoryFromSystem(AllocationSystem::TWO_GIGABYTE));
    ProxyAllocator freeListAllocator(allocator1);
    ProxyAllocator poolAllocator(allocator2);

    a* ptr1 = allocateNew<a>(poolAllocator);
    b obj1 = {
        allocateArray<float>(freeListAllocator, 1024)
    };

    ptr1->i = 5;
    ptr1->x = 6.f;
    ptr1->y = 7.f;
    ptr1->z = 8.f;

    obj1.data[303] = 100.f;

    a* ptr2 = allocateNew<a>(poolAllocator);
    b obj2 = {
        allocateArray<float>(freeListAllocator, 1024)
    };

    obj2.data[202] = 23.f;
    ptr2->i = 5;
    ptr2->x = 6.f;
    ptr2->y = 7.f;
    ptr2->z = 8.f;
    poolAllocator.deallocate(ptr1);
    poolAllocator.deallocate(ptr2);
    deallocateArray(freeListAllocator, obj1.data);
    deallocateArray(freeListAllocator, obj2.data);
    return 0;
}
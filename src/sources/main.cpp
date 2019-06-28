#include <iostream>

#include <LinearAllocator.hpp>
#include <StackAllocator.hpp>
#include <FreeListAllocator.hpp>
#include <PoolAllocator.hpp>
#include <ProxyAllocator.hpp>
#include <AllocationSystem.hpp>

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
	void* allocatorMemory1 = nullptr;
	AllocationSystem::requestMemoryFromSystem(AllocationSystem::TWO_GIGABYTE)
		.map([&allocatorMemory1](void* ptr) { allocatorMemory1 = ptr; });
	void* allocatorMemory2 = nullptr;
	AllocationSystem::requestMemoryFromSystem(AllocationSystem::TWO_GIGABYTE)
		.map([&allocatorMemory2](void* ptr) { allocatorMemory2 = ptr; });


    FreeListAllocator allocator1(AllocationSystem::TWO_GIGABYTE, allocatorMemory1);
    PoolAllocator allocator2(sizeof(a), alignof(a), AllocationSystem::TWO_GIGABYTE, allocatorMemory2);
    ProxyAllocator freeListAllocator(allocator1);
    ProxyAllocator poolAllocator(allocator2);

	a* ptr1 = nullptr;
	allocateNew<a>(poolAllocator)
		.map([&ptr1](a* ptr) { ptr1 = ptr; });
    
	b obj1 = {
        nullptr
    };

	allocateArray<float>(freeListAllocator, 1024)
		.map([&obj1](float* ptr) { obj1.data = ptr; });

    ptr1->i = 5;
    ptr1->x = 6.f;
    ptr1->y = 7.f;
	ptr1->z = 8.f;

	obj1.data[303] = 100.f;

	a* ptr2 = nullptr;
	allocateNew<a>(poolAllocator)
		.map([&ptr2](a* ptr) { ptr2 = ptr; });

	b obj2 = {
		nullptr
    };
	allocateArray<float>(freeListAllocator, 1024)
		.map([&obj2](float* ptr) { obj2.data = ptr; });

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
#include <AllocationSystem.hpp>

using namespace puggo;

AllocationSystem AllocationSystem::system;

AllocationSystem::AllocationSystem(void) {}

AllocationSystem::AllocationSystem(unsigned long long size, size_t sizePerAllocator) {
    initMemory(size, sizePerAllocator);
}

AllocationSystem::~AllocationSystem(void) {
    freeAllMemory();
}
void AllocationSystem::initMemory(unsigned long long size, size_t sizePerAllocator) {
#ifdef _WIN64
    assert(size <= (SIXTEEN_GIGABYTE * 2));
#else
    assert(size <= ONE_GIGABYTE);
#endif // _WIN32

    size_t numAllocators = static_cast<size_t>(ceil(static_cast<float>(size) / static_cast<float>(sizePerAllocator)));

    for (size_t i = 0; i < numAllocators - 1; i++) {
        dataArr.push_back(malloc(sizePerAllocator));
        masterAllocators.push_back(LinearAllocator(sizePerAllocator, dataArr.back()));
    }

    dataArr.push_back(malloc(static_cast<size_t>(size - (numAllocators - 1) * static_cast<unsigned long long>(sizePerAllocator))));
    masterAllocators.push_back(LinearAllocator(static_cast<size_t>(size - (numAllocators - 1) * static_cast<unsigned long long>(sizePerAllocator)), dataArr.back()));
}

AllocationSystem& AllocationSystem::operator=(AllocationSystem&& system) noexcept {
    freeAllMemory();
    this->dataArr = move(system.dataArr);
    this->masterAllocators = move(system.masterAllocators);

    return *this;
}

void AllocationSystem::init(unsigned long long size, size_t sizePerAllocator) {
    AllocationSystem::system = AllocationSystem(size, sizePerAllocator);
}

void* AllocationSystem::requestMemoryFromSystem(size_t size) {
    return AllocationSystem::system.requestMemory(size);
}

void* AllocationSystem::requestMemory(size_t size) {
    assert(size <= static_cast<size_t>(AllocationSystem::TWO_GIGABYTE));

    int indexOfAllocator = -1;
    for (size_t i = 0; i < masterAllocators.size(); i++) {
        LinearAllocator& allocator = masterAllocators[i];
        if (size <= allocator.getSize() - allocator.getUsedMemory()) {
            indexOfAllocator = static_cast<int>(i);
            break;
        }
    }

    if (indexOfAllocator == -1) {
        return nullptr;
    }

    return masterAllocators[indexOfAllocator].allocate(size, 1);
}

void AllocationSystem::freeAllMemory(void) {
    for_each(masterAllocators.begin(), masterAllocators.end(), [](LinearAllocator& allocator) { allocator.clear(); });
    for_each(dataArr.begin(), dataArr.end(), [](void* data) { free(data); });
    masterAllocators.clear();
    dataArr.clear();
}
#include <AllocationSystem.hpp>

using namespace puggo;

AllocationSystem AllocationSystem::system;

AllocationSystem::AllocationSystem(void) {}

AllocationSystem::AllocationSystem(unsigned long long size, size_t sizePerAllocator) {
    initMemory(size, sizePerAllocator);
}

AllocationSystem::~AllocationSystem(void) {
    cout << "~AllocationSystem" << endl;
    freeAllMemory();
}
void AllocationSystem::initMemory(unsigned long long size, size_t sizePerAllocator) {
    cout << "initMemory" << endl;
    cout << "size: " << size << endl;
    cout << "sizePerAllocator: " << sizePerAllocator << endl;
#ifdef _WIN64
    assert(size <= (SIXTEEN_GIGABYTE * 2));
#else
    assert(size <= ONE_GIGABYTE);
#endif // _WIN32

    size_t numAllocators = static_cast<size_t>(ceil(static_cast<float>(size) / static_cast<float>(sizePerAllocator)));
    cout << "numAllocators: " << numAllocators << endl;
    for (size_t i = 0; i < numAllocators - 1; i++) {
        cout << "malloc call" << endl;
        void* data = malloc(sizePerAllocator);
        cout << "data: " << data << endl;
        dataArr.push_back(data);
        cout << "data pushed back" << endl;
        masterAllocators.push_back(LinearAllocator(sizePerAllocator, data));
        cout << "Linear allocator pushed back" << endl;
    }

    cout << "malloc call" << endl;
    void* lastData = malloc(static_cast<size_t>(size - (numAllocators - 1) * static_cast<unsigned long long>(sizePerAllocator)));
    cout << "lastData: " << lastData << endl;
    dataArr.push_back(lastData);
    cout << "lastData pushed back" << endl;
    masterAllocators.push_back(LinearAllocator(static_cast<size_t>(size - (numAllocators - 1) * static_cast<unsigned long long>(sizePerAllocator)), lastData));
    cout << "initMemory::succeeded" << endl;
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
    cout << "freeAllMemory" << endl;
    for_each(masterAllocators.begin(), masterAllocators.end(), [](LinearAllocator& allocator) { allocator.clear(); });
    for_each(dataArr.begin(), dataArr.end(), [](void* data) { free(data); });
    masterAllocators.clear();
    dataArr.clear();
    cout << "cleared" << endl;
}
using namespace puggo;

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle>
GenericComponentAllocator<T, IndexSize, GenerationSize, Handle> GenericComponentAllocator<T, IndexSize, GenerationSize, Handle>::componentAllocator(0);

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle>
GenericComponentAllocator<T, IndexSize, GenerationSize, Handle>::GenericComponentAllocator(const uint32_t& genericComponentLimit) {
    if (genericComponentLimit > 0) {
        cout << "genericComponentLimit: " << genericComponentLimit << endl;
        assert(genericComponentLimit <= static_cast<uint32_t>(pow(2, IndexSize)));

        uint32_t size = (genericComponentLimit + 4) * (sizeof(uint32_t) + sizeof(T));
        allocatedMemory = AllocationSystem::requestMemoryFromSystem(size);
        LinearAllocator allocator(size, allocatedMemory);

        this->genericComponentLimit = genericComponentLimit;
        generation = allocateArray<uint32_t>(allocator, genericComponentLimit);
        objects = allocateArray<T>(allocator, genericComponentLimit);
        allocator.clear();

        if (this->genericComponentLimit < minimumFreeIndices) {
            minimumFreeIndices = static_cast<uint32_t>(this->genericComponentLimit * 0.25);
        }

        freeIndices = deque<uint32_t>(minimumFreeIndices);
        freeIndices.clear();
        cout << "Leaving scope" << endl;
    }
    cout << "Exiting constructor" << endl;
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle>
GenericComponentAllocator<T, IndexSize, GenerationSize, Handle>::GenericComponentAllocator(GenericComponentAllocator&& componentAllocator) noexcept {
    allocatedMemory = move(componentAllocator.allocatedMemory);
    freeIndices = move(componentAllocator.freeIndices);
    genericComponentLimit = componentAllocator.genericComponentLimit;
    minimumFreeIndices = componentAllocator.minimumFreeIndices;
    generationSize = componentAllocator.generationSize;
    generation = move(componentAllocator.generation);
    objects = move(componentAllocator.objects);
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle>
GenericComponentAllocator<T, IndexSize, GenerationSize, Handle>& GenericComponentAllocator<T, IndexSize, GenerationSize, Handle>::operator=(GenericComponentAllocator&& transformAllocator) noexcept {
    allocatedMemory = move(componentAllocator.allocatedMemory);
    freeIndices = move(componentAllocator.freeIndices);
    genericComponentLimit = componentAllocator.genericComponentLimit;
    minimumFreeIndices = componentAllocator.minimumFreeIndices;
    generationSize = componentAllocator.generationSize;
    generation = move(componentAllocator.generation);
    objects = move(componentAllocator.objects);

    return *this;
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle>
void GenericComponentAllocator<T, IndexSize, GenerationSize, Handle>::init(const uint32_t& genericComponentLimit) {
    GenericComponentAllocator::deallocate();
    GenericComponentAllocator::componentAllocator = GenericComponentAllocator<T, IndexSize, GenerationSize, Handle>(genericComponentLimit);
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle>
void GenericComponentAllocator<T, IndexSize, GenerationSize, Handle>::deallocate(void) {
    GenericComponentAllocator::componentAllocator.freeMemory();
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle>
Handle GenericComponentAllocator<T, IndexSize, GenerationSize, Handle>::create(void) {
    return GenericComponentAllocator::componentAllocator.createInternal();
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle>
T& GenericComponentAllocator<T, IndexSize, GenerationSize, Handle>::get(const Handle& handle) {
    return GenericComponentAllocator::componentAllocator.getInternal(handle);
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle>
void GenericComponentAllocator<T, IndexSize, GenerationSize, Handle>::destroy(const Handle& handle) {
    GenericComponentAllocator::componentAllocator.destroyInternal(handle);
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle>
bool GenericComponentAllocator<T, IndexSize, GenerationSize, Handle>::isInitialized(void) {
    return GenericComponentAllocator::componentAllocator.isInitializedInternal();
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle>
bool GenericComponentAllocator<T, IndexSize, GenerationSize, Handle>::isValidHandle(const Handle& handle) {
    return GenericComponentAllocator::componentAllocator.isValidHandleInternal(handle);
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle>
Handle GenericComponentAllocator<T, IndexSize, GenerationSize, Handle>::createInternal(void) {
    uint32_t idx;
    if (freeIndices.size() >= minimumFreeIndices) {
        idx = freeIndices.front();
        freeIndices.pop_front();
    }
    else {
        assert(generationSize < genericComponentLimit);
        idx = generationSize++;
        generation[idx] = 0;
    }

    objects[idx] = T();

    return {
        idx,
        generation[idx]
    };
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle>
T& GenericComponentAllocator<T, IndexSize, GenerationSize, Handle>::getInternal(const Handle& handle) {
    assert(isValidHandleInternal(handle));
    return objects[handle.index];
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle>
void GenericComponentAllocator<T, IndexSize, GenerationSize, Handle>::destroyInternal(const Handle& handle) {
    assert(isValidHandleInternal(handle));

    ++generation[handle.index];
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle>
bool GenericComponentAllocator<T, IndexSize, GenerationSize, Handle>::isInitializedInternal(void) {
    return componentAllocator.allocatedMemory != nullptr;
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle>
void GenericComponentAllocator<T, IndexSize, GenerationSize, Handle>::freeMemory(void) {
    cout << "freeMemory" << endl;
    if (componentAllocator.allocatedMemory != nullptr) {
        generationSize = 0;
        freeIndices.clear();

        allocatedMemory = nullptr;
        generation = nullptr;
        objects = nullptr;
    }
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle>
bool GenericComponentAllocator<T, IndexSize, GenerationSize, Handle>::isValidHandleInternal(const Handle& handle) {
    return generation[handle.index] == handle.generation;
}
using namespace puggo;

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle, typename... Args>
GenericComponentAllocator<T, IndexSize, GenerationSize, Handle, Args...>::GenericComponentAllocator(void) noexcept {}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle, typename... Args>
GenericComponentAllocator<T, IndexSize, GenerationSize, Handle, Args...>::GenericComponentAllocator(const uint32_t& genericComponentLimit) {
    assert(genericComponentLimit <= static_cast<uint32_t>(pow(2, IndexSize)));

    uint32_t size = (genericComponentLimit + 1) * (sizeof(uint32_t) + sizeof(T));
    allocatedMemory = AllocationSystem::requestMemoryFromSystem(size);
    LinearAllocator allocator(size, data);

    this->genericComponentLimit = genericComponentLimit;
    generation = allocateArray<uint32_t>(allocator, size);
    transform = allocateArray<T, Args...>(allocator, size);

    if (this->genericComponentLimit < minimumFreeIndices) {
        minimumFreeIndices = static_cast<uint32_t>(this->genericComponentLimit * 0.25);
    }

    freeIndices = deque<uint32_t>(minimumFreeIndices);
    freeIndices.clear();
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle, typename... Args>
GenericComponentAllocator<T, IndexSize, GenerationSize, Handle, Args...>::GenericComponentAllocator(GenericComponentAllocator&& componentAllocator) noexcept {
    allocator = move(componentAllocator.allocator);
    freeIndices = move(componentAllocator.freeIndices);
    genericComponentLimit = componentAllocator.genericComponentLimit;
    minimumFreeIndices = componentAllocator.minimumFreeIndices;
    generationSize = componentAllocator.generationSize;
    generation = move(componentAllocator.generation);
    objects = move(componentAllocator.objects);
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle, typename... Args>
GenericComponentAllocator<T, IndexSize, GenerationSize, Handle, Args...>& GenericComponentAllocator<T, IndexSize, GenerationSize, Handle, Args...>::operator=(GenericComponentAllocator&& transformAllocator) noexcept {
    allocator = move(componentAllocator.allocator);
    freeIndices = move(componentAllocator.freeIndices);
    genericComponentLimit = componentAllocator.genericComponentLimit;
    minimumFreeIndices = componentAllocator.minimumFreeIndices;
    generationSize = componentAllocator.generationSize;
    generation = move(componentAllocator.generation);
    objects = move(componentAllocator.objects);

    return *this;
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle, typename... Args>
void GenericComponentAllocator<T, IndexSize, GenerationSize, Handle, Args...>::init(const uint32_t& genericComponentLimit) {
    GenericComponentAllocator::deallocate();
    GenericComponentAllocator::componentAllocator = GenericComponentAllocator<T, Args...>(genericComponentLimit);
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle, typename... Args>
void GenericComponentAllocator<T, IndexSize, GenerationSize, Handle, Args...>::deallocate(void) {
    GenericComponentAllocator::componentAllocator.freeMemory();
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle, typename... Args>
Handle GenericComponentAllocator<T, IndexSize, GenerationSize, Handle, Args...>::create(Args&&... args) {
    return GenericComponentAllocator::componentAllocator.createInternal(forward<Args>(args)...);
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle, typename... Args>
T& GenericComponentAllocator<T, IndexSize, GenerationSize, Handle, Args...>::get(const Handle& handle) {
    return GenericComponentAllocator::componentAllocator.getInternal(handle);
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle, typename... Args>
void GenericComponentAllocator<T, IndexSize, GenerationSize, Handle, Args...>::destroy(const Handle& handle) {
    GenericComponentAllocator::componentAllocator.destroyInternal(handle);
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle, typename... Args>
bool GenericComponentAllocator<T, IndexSize, GenerationSize, Handle, Args...>::isInitialized(void) {
    return GenericComponentAllocator::componentAllocator.isInitializedInternal();
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle, typename... Args>
bool GenericComponentAllocator<T, IndexSize, GenerationSize, Handle, Args...>::isValidHandle(const Handle& handle) {
    return GenericComponentAllocator::componentAllocator.isValidHandleInternal(handle);
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle, typename... Args>
Handle GenericComponentAllocator<T, IndexSize, GenerationSize, Handle, Args...>::createInternal(Args&&... args) {
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

    objects[idx] = T(forward<Args>(args)...);

    return {
        idx,
        generation[idx]
    };
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle, typename... Args>
T& GenericComponentAllocator<T, IndexSize, GenerationSize, Handle, Args...>::getInternal(const Handle& handle) {
    assert(isValidHandleInternal(handle));
    return objects[handle.index];
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle, typename... Args>
void GenericComponentAllocator<T, IndexSize, GenerationSize, Handle, Args...>::destroyInternal(const Handle& handle) {
    assert(isValidHandleInternal(handle));

    ++generation[handle.index];
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle, typename... Args>
bool GenericComponentAllocator<T, IndexSize, GenerationSize, Handle, Args...>::isInitializedInternal(void) {
    return componentAllocator.allocatedMemory != nullptr;
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle, typename... Args>
void GenericComponentAllocator<T, IndexSize, GenerationSize, Handle, Args...>::freeMemory(void) {
    if (componentAllocator.allocatedMemory != nullptr) {
        generationSize = 0;
        freeIndices.clear();

        allocatedMemory = nullptr;
        generation = nullptr;
        transform = nullptr;
    }
}

template <typename T, uint8_t IndexSize, uint8_t GenerationSize, typename Handle, typename... Args>
bool GenericComponentAllocator<T, IndexSize, GenerationSize, Handle, Args...>::isValidHandleInternal(const Handle& handle) {
    return generation[handle.index] == handle.generation;
}
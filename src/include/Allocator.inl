template <typename T>
T* puggo::allocateNew(Allocator& allocator) {
    return new (allocator.allocate(sizeof(T), alignof(T))) T;
}

template <typename T>
T* puggo::allocateNew(Allocator& allocator, const T& t) {
    return new (allocator.allocate(sizeof(T), alignof(T))) T(t);
}

template <typename T>
void puggo::deallocateDelete(Allocator& allocator, T* object) {
    object->~T();
    allocator.deallocate(object);
}

template <typename T>
T* puggo::allocateArray(Allocator& allocator, size_t length) {
    assert(length != 0);
    unsigned char headerSize = sizeof(size_t) / sizeof(T);

    if (sizeof(size_t) % sizeof(T) > 0) {
        headerSize += 1;
    }

    // Allocate extra space to store array length in the bytes before the array
    T* p = ((T*)allocator.allocate(sizeof(T) * (length + headerSize), alignof(T))) + headerSize;
    *(((size_t*)p) - 1) = length;

    for (size_t i = 0; i < length; i++) {
        new (&p) T;
    }

    return p;
}

template <typename T>
void puggo::deallocateArray(Allocator& allocator, T* array) {
    assert(array != nullptr);
    size_t length = *(((size_t*)array) - 1);

    /*for (size_t i = 0; i < length; i++) {
        array.~T();
    }*/

    // Calculate how much extra memory was allocated to store the length before the array
    unsigned char headerSize = sizeof(size_t) / sizeof(T);
    if (sizeof(size_t) % sizeof(T) > 0) {
        headerSize += 1;
    }

    allocator.deallocate(array - headerSize);
}

unsigned char puggo::alignForwardAdjustment(const void* address, const unsigned char& alignment) {
    unsigned char adjustment = alignment - static_cast<unsigned char>(reinterpret_cast<size_t>(address) & static_cast<size_t>(alignment - 1));
    if (adjustment == alignment) {
        return 0;
    }

    return adjustment;
}

unsigned char puggo::alignForwardAdjustmentWithHeader(
    const void* address,
    const unsigned char& alignment,
    const unsigned char& headerSize
) {
    unsigned char adjustment = alignForwardAdjustment(address, alignment);
    unsigned char neededSpace = headerSize;

    if (adjustment < neededSpace) {
        neededSpace -= adjustment;

        // Increase adjustment to fit header
        adjustment += alignment * (neededSpace / alignment);

        if (neededSpace % alignment > 0) {
            adjustment += alignment;
        }
    }

    return adjustment;
}
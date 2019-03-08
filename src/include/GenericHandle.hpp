#ifndef PUGGO_GENERIC_HANDLE
#define PUGGO_GENERIC_HANDLE

#include <type_traits>
#include <iostream>

using namespace std;

namespace puggo {
    template <
        uint8_t IndexSize,
        uint8_t GenerationSize
    >
    struct GenericHandle {
        uint32_t index : IndexSize;
        uint32_t generation : GenerationSize;

        bool operator==(const GenericHandle<IndexSize, GenerationSize>& handle) {
            return index == handle.index && generation == handle.generation;
        }
    };

    template <
        uint8_t IndexSize,
        uint8_t GenerationSize
    >
    bool operator==(const GenericHandle<IndexSize, GenerationSize>& left, const GenericHandle<IndexSize, GenerationSize>& right) {
        return left.index == right.index && left.generation == right.generation;
    };
}

#endif // !PUGGO_GENERIC_HANDLE

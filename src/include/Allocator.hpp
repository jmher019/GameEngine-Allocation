#ifndef PUGGO_ALLOCATOR_HPP
#define PUGGO_ALLOCATOR_HPP

#include <assert.h>

#include <Result.hpp>

namespace puggo {
    class Allocator {
    public:
        Allocator(const size_t& size, void* start);

        virtual ~Allocator(void);

        virtual Result<void*, nullptr_t> allocate(const size_t& size, const unsigned char& alignment = 4) = 0;
        virtual void deallocate(void* p) = 0;
		void* getStart(void) const;
        size_t getSize(void) const;
        size_t getUsedMemory(void) const;
        size_t getNumAllocations(void) const;

    protected:
        void* start;
        size_t size;
        size_t usedMemory;
        size_t numAllocations;
    };

    template <typename T>
	inline Result<T*, nullptr_t> allocateNew(Allocator& allocator) {
		Result<T*, nullptr_t> result = Result<T*, nullptr_t>::error(nullptr);
		allocator
			.allocate(sizeof(T), alignof(T))
			.map([&result] (void* ptr) {
				result = Result<T*, nullptr_t>::ok(new(ptr) T);
			});

		return result;
	}

    template <typename T>
	inline Result<T*, nullptr_t> allocateNew(Allocator& allocator, const T& t) {
		Result<T*, nullptr_t> result = Result<T*, nullptr_t>::error(nullptr);
		allocator
			.allocate(sizeof(T), alignof(T))
			.map([&result](void* ptr) {
				result = Result<T*, nullptr_t>::ok(new(ptr) T(t));
			});

		return result;
	}

    template <typename T>
	inline void deallocateDelete(Allocator& allocator, T* object) {
		object->~T();
		allocator.deallocate(object);
	}

    template <typename T>
	inline Result<T*, nullptr_t> allocateArray(Allocator& allocator, size_t length) {
		if (length == 0) {
			return Result<T*, nullptr_t>::error(nullptr);
		}

		unsigned char headerSize = sizeof(size_t) / sizeof(T);

		if (sizeof(size_t) % sizeof(T) > 0) {
			headerSize += 1;
		}

		// Allocate extra space to store array length in the bytes before the array
		Result<T*, nullptr_t> result = Result<T*, nullptr_t>::error(nullptr);
		allocator.allocate(sizeof(T) * (length + headerSize), alignof(T))
			.map([&result, &headerSize, &length] (void*ptr) {
				T* p = ((T*)ptr) + headerSize;
				*(((size_t*)p) - 1) = length;

				for (size_t i = 0; i < length; i++) {
					new (&p) T;
				}

				result = Result<T*, nullptr_t>::ok(p);
			});

		return result;
	}

    template <typename T>
	inline void deallocateArray(Allocator& allocator, T* array) {
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

	inline unsigned char alignForwardAdjustment(const void* address, const unsigned char& alignment) {
		unsigned char adjustment = alignment - static_cast<unsigned char>(reinterpret_cast<size_t>(address) & static_cast<size_t>(alignment - 1));
		if (adjustment == alignment) {
			return 0;
		}

		return adjustment;
	}

	inline unsigned char alignForwardAdjustmentWithHeader(
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
}

#endif // !PUGGO_ALLOCATOR_HPP

#include <FreeListAllocator.hpp>

using namespace puggo;

FreeListAllocator::FreeListAllocator(size_t size, void* start):
    Allocator(size, start),
    freeBlocks((FreeBlock*)start) {

    assert(size > sizeof(FreeBlock));
    freeBlocks->size = size;
    freeBlocks->next = nullptr;
}

FreeListAllocator::FreeListAllocator(FreeListAllocator&& allocator) noexcept:
    Allocator(move(allocator.size), move(allocator.start)) {
    numAllocations = move(allocator.numAllocations);
    usedMemory = move(allocator.usedMemory);
    freeBlocks = move(allocator.freeBlocks);
}

FreeListAllocator::~FreeListAllocator(void) {
    freeBlocks = nullptr;
}

Result<void*, nullptr_t> FreeListAllocator::allocate(const size_t& size, const unsigned char& alignment) {
	if (size == 0 || alignment == 0) {
		return Result<void*, nullptr_t>::error(nullptr);
	}

    FreeBlock* prevFreeBlock = nullptr;
    FreeBlock* freeBlock = freeBlocks;

    while (freeBlock != nullptr) {
        // Calculate adjustment needed to keep object correctly aligned
        const unsigned char adjustment = alignForwardAdjustmentWithHeader(freeBlock, alignment, sizeof(AllocationHeader));
        size_t totalSize = size + adjustment;

        // If allocation doesn't fit in this FreeBlock, try the next
        if (freeBlock->size < totalSize) {
            prevFreeBlock = freeBlock;
            freeBlock = freeBlock->next;
            continue;
        }

        static_assert(sizeof(AllocationHeader) >= sizeof(FreeBlock), "sizeof(AllocationHeder) < sizeof(FreeBlock)");

        // If allocations in the remaining memory will be impossible
        if (freeBlock->size - totalSize <= sizeof(AllocationHeader)) {
            // Increase allocation size instead of creating a new FreeBlock
            totalSize = freeBlock->size;

            if (prevFreeBlock != nullptr) {
                prevFreeBlock->next = freeBlock->next;
            }
            else {
                freeBlocks = freeBlock->next;
            }
        }
        else {
            // Else create a new FreeBlock containing memory
            FreeBlock* nextBlock = (FreeBlock*)((unsigned char*)freeBlock + totalSize);
            
            nextBlock->size = freeBlock->size - totalSize;
            nextBlock->next = freeBlock->next;

            if (prevFreeBlock != nullptr) {
                prevFreeBlock->next = freeBlock->next;
            }
            else {
                freeBlocks = nextBlock;
            }
        }

        unsigned char* alignedAddress = (unsigned char*)freeBlock + adjustment;
        AllocationHeader* header = (AllocationHeader*)(alignedAddress - sizeof(AllocationHeader));
        header->size = totalSize;
        header->adjustment = adjustment;
        usedMemory += totalSize;
        numAllocations++;

		if (alignForwardAdjustment((void*)alignedAddress, alignment) != 0) {
			return Result<void*, nullptr_t>::error(nullptr);
		}

		return Result<void*, nullptr_t>::ok((void*)alignedAddress);
    }

	return Result<void*, nullptr_t>::error(nullptr);
}

void FreeListAllocator::deallocate(void* p) {
    assert(p != nullptr);
    AllocationHeader* header = (AllocationHeader*)((unsigned char*)p - sizeof(AllocationHeader));
    unsigned char* blockStart = reinterpret_cast<unsigned char*>(p) - header->adjustment;
    size_t blockSize = header->size;
    unsigned char* blockEnd = blockStart + blockSize;
    FreeBlock* prevFreeBlock = nullptr;
    FreeBlock* freeBlock = freeBlocks;

    while (freeBlock != nullptr) {
        if ((unsigned char*) freeBlock >= blockEnd) {
            break;
        }

        prevFreeBlock = freeBlock;
        freeBlock = freeBlock->next;
    }

    if (prevFreeBlock == nullptr) {
        prevFreeBlock = (FreeBlock*)blockStart;
        prevFreeBlock->size = blockSize;
        prevFreeBlock->next = freeBlocks;
        freeBlocks = prevFreeBlock;
    }
    else if ((unsigned char*)prevFreeBlock + prevFreeBlock->size == blockStart) {
        prevFreeBlock->size += blockSize;
    }
    else {
        FreeBlock* temp = (FreeBlock*)blockStart;
        temp->size = blockSize;
        temp->next = prevFreeBlock->next;
        prevFreeBlock->next = temp;
        prevFreeBlock = temp;
    }

    if (freeBlock != nullptr && (unsigned char*)freeBlock == blockEnd) {
        prevFreeBlock->size += freeBlock->size;
        prevFreeBlock->next = freeBlock->next;
    }

    numAllocations--;
    usedMemory -= blockSize;
}
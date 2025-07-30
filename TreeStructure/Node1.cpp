#include "../Node1.h"

MemoryPool* MemoryPool::poolInstance = nullptr;

MemoryPool* MemoryPool::createInstance() {

    if (poolInstance == nullptr)
    {
        poolInstance = new MemoryPool(2ull * 1024 * 1024 * 1024); // 2GB pool
    }

    return poolInstance;
}

void* MemoryPool::allocate(size_t size, size_t alignment) {
    auto& freeList = freeChunkPoolMemory[size];
    if (!freeList.empty()) {
        void* ptr = freeList.back();
        freeList.pop_back();
        return ptr;
    }
    size_t current = reinterpret_cast<size_t>(pool) + offset;
    size_t aligned = (current + alignment - 1) & ~(alignment - 1);
    size_t newOffset = aligned - reinterpret_cast<size_t>(pool) + size;
    if (newOffset > poolSize) throw std::bad_alloc();
    void* ptr = pool + (aligned - reinterpret_cast<size_t>(pool));
    offset = newOffset;
    return ptr;
}

void MemoryPool::deallocate(void* ptr, size_t size) {
    freeChunkPoolMemory[size].push_back(ptr);
}

void MemoryPool::reset() { offset = 0; }
size_t MemoryPool::getOffset() const { return offset; }
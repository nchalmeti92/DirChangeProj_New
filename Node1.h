//#include <iostream>
//#include <cstring>
//#include <cstdlib>
//#include <vector>
//#include <chrono>
//#include <string>
//#include <cmath>


//static const std::string fixedName = "abcdefghij"; // 10 chars
//static const int offSetCount = 2; // Example offset count
//static const int fanout = 10; // Example fanout
//static bool withName = false; // Example flag for name
//static bool withOffsets = false; // Example flag for offsets
//static int nodeCount = 0;
//
//struct Node1 {
//    void* temp1;     // currentOffset starting address
//    void* temp2;     // prevOffsets starting address
//    void* temp3;     // children starting address
//    char* name;
//
//    uint32_t size3;  // count of children
//    uint8_t size1;   // currentOffset size
//    uint8_t size2;   // prevOffset size
//    uint8_t flags = 0;
//    uint8_t flags2 = 0;
//
//
//    Node1()
//        : temp1(nullptr), temp2(nullptr), temp3(nullptr),
//        name(nullptr), size3(0), size1(0), size2(0) {
//    }
//
//    ~Node1() {
//        free(temp1);
//        free(temp2);
//        free(temp3);
//        free(name);
//    }
//
//    
//    void setName() {  
//        if (fixedName.empty()) return;
//        name = (char*)malloc(fixedName.size() + 1);
//        if (name) {  
//            strcpy_s(name, fixedName.size() + 1, fixedName.c_str());
//        }  
//    }
//
//    void allocateOffsets() {
//        size1 = offSetCount;
//        size2 = offSetCount;
//        if (offSetCount > 0) {
//            temp1 = malloc(offSetCount * sizeof(uint64_t)); // example offset size
//            std::memset(temp1, 0, offSetCount * sizeof(uint64_t));
//        }
//        if (offSetCount > 0) {
//            temp2 = malloc(offSetCount * sizeof(uint64_t));
//            std::memset(temp2, 0, offSetCount * sizeof(uint64_t));
//        }
//    }
//
//    void allocateChildren() {
//        size3 = fanout;
//        if (fanout > 0) {
//            temp3 = malloc(fanout * sizeof(Node1*)); // Array of Node1* pointers
//            std::memset(temp3, 0, fanout * sizeof(Node1*));
//        }
//    }
//
//    Node1** children() {
//        return reinterpret_cast<Node1**>(temp3);
//    }
//};
//
//// Recursively build a tree with the given fanout and depth, returning the total node count
//size_t buildTree(Node1* node, int depth) {
//    if (withName) node->setName(); // 10 chars
//    if (withOffsets) node->allocateOffsets();
//    
//    nodeCount++;
//    if (depth == 0) return 1;
//    node->allocateChildren();
//    for (int i = 0; i < fanout; ++i) {
//        Node1* child = new Node1();
//        node->children()[i] = child;
//        buildTree(child, depth - 1);
//    }
//    return nodeCount;
//}
//
//void freeTree(Node1* node) {
//    if (!node) return;
//    for (uint32_t i = 0; i < node->size3; ++i) {
//        freeTree(node->children()[i]);
//    }
//    delete node;
//}
//
//void testCase(const std::string& label, int fanout, int depth, bool withname, bool withoffsets) {
//    std::cout << label << std::endl;
//    auto start = std::chrono::high_resolution_clock::now();
//
//	withName = withname;
//	withOffsets = withoffsets;
//
//    Node1* root = new Node1();
//    size_t nodeCount = 0;
//    buildTree(root, depth);
//
//    auto end = std::chrono::high_resolution_clock::now();
//    std::chrono::duration<double> duration = end - start;
//    std::cout << "Created " << nodeCount << " nodes in " << duration.count() << " seconds.\n";
//
//    freeTree(root);
//}

//int main() {
//    // 10^7 = 10,000,000, so depth=6, fanout=10 gives 1+10+100+1000+10000+100000+1000000=1,111,111 nodes
//    // To get close to 10 million, use depth=7, fanout=10: sum_{i=0}^7 10^i = 11,111,111
//    testCase("Tree: 10 children per node, depth 7", 10, 7);
//    return 0;
//}


#pragma once
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <chrono>
#include <string>
#include <cmath>
#include <cassert>
#include <queue>
#include <unordered_map>

// Simple bump-pointer memory pool
class MemoryPool {
public:
    MemoryPool(size_t size) : poolSize(size), offset(0) {
        pool = (char*)std::malloc(poolSize);
        if (!pool) throw std::bad_alloc();
    }
    ~MemoryPool() {
        std::free(pool);
    }
    void* allocate(size_t size, size_t alignment = alignof(std::max_align_t)) {
        auto& freeList = freeChunkPoolMemory[size];
        if (!freeList.empty()) {
            void* ptr = freeList.back();
            freeList.pop_back();
            return ptr;
        }
        // Normal bump-pointer allocation
        size_t current = reinterpret_cast<size_t>(pool) + offset;
        size_t aligned = (current + alignment - 1) & ~(alignment - 1);
        size_t newOffset = aligned - reinterpret_cast<size_t>(pool) + size;
        if (newOffset > poolSize) throw std::bad_alloc();
        void* ptr = pool + (aligned - reinterpret_cast<size_t>(pool));
        offset = newOffset;
        return ptr;
    }

    void deallocate(void* ptr, size_t size) {
        freeChunkPoolMemory[size].push_back(ptr);
    }

    void reset() { offset = 0; }

    size_t getOffset() const {
        return offset;
    }
private:
    char* pool;
    size_t poolSize;
    size_t offset;

    std::unordered_map<size_t, std::vector<void*>> freeChunkPoolMemory;
};

// Global pool instance (tune size as needed)
static MemoryPool gPool(2ull * 1024 * 1024 * 1024); // 2GB pool

static const std::string fixedName = "abcdefghij"; // 10 chars
static const int offSetCount = 2;
static const int fanout = 10;
static bool withName = false;
static bool withOffsets = false;
static int nodeCount = 0;

struct Node1 {
    void* currentOffsets;
    void* previousOffsets;
    void* childNodes;
    char* name;

    uint32_t childCount;
    uint8_t currentOffsetSize;
    uint8_t previousOffsetSize;
    uint16_t flags = 0;

    Node1() : currentOffsets(nullptr), previousOffsets(nullptr), childNodes(nullptr),
        name(nullptr), childCount(0), currentOffsetSize(0), previousOffsetSize(0) {
    }

    ~Node1() {
        // No free: memory is managed by the pool and released all at once  
    }

    void setName() {
        name = (char*)gPool.allocate(fixedName.size() + 1, alignof(char));
        strcpy_s(name, fixedName.size() + 1, fixedName.c_str());
    }

    void setName(const std::string& inputname) {
        name = (char*)gPool.allocate(inputname.size() + 1, alignof(char));
        strcpy_s(name, inputname.size() + 1, inputname.c_str());
    }

    void allocateOffsets() {
        currentOffsetSize = offSetCount;
        previousOffsetSize = offSetCount;
        if (offSetCount > 0) {
            currentOffsets = gPool.allocate(offSetCount * sizeof(uint64_t), alignof(uint64_t));
            std::memset(currentOffsets, 0, offSetCount * sizeof(uint64_t));
        }
        if (offSetCount > 0) {
            previousOffsets = gPool.allocate(offSetCount * sizeof(uint64_t), alignof(uint64_t));
            std::memset(previousOffsets, 0, offSetCount * sizeof(uint64_t));
        }
    }

    void allocateChildren() {
        childCount = fanout;
        if (fanout > 0) {
            childNodes = gPool.allocate(fanout * sizeof(Node1*), alignof(Node1*));
            std::memset(childNodes, 0, fanout * sizeof(Node1*));
        }
    }

    Node1* AddAChild(const std::string& name) {
        Node1* child = new (gPool.allocate(sizeof(Node1), alignof(Node1))) Node1();
        addChild(child);
        child->setName(name);
        return child;
    }

    void addChild(Node1* child) {
        Node1** newChildren = nullptr;
        try {
            newChildren = reinterpret_cast<Node1**>(gPool.allocate((childCount + 1) * sizeof(Node1*), alignof(Node1*)));
        }
        catch (const std::bad_alloc& e) {
            std::cerr << "Memory allocation failed: " << e.what() << std::endl;
            return;
        }

        for (uint32_t i = 0; i < childCount; ++i) {
            newChildren[i] = children()[i];
        }

        if (childCount > 0)
        {
            gPool.deallocate(childNodes, childCount * sizeof(Node1*));
        }

        newChildren[childCount] = child;
        childNodes = newChildren;
        childCount += 1;
    }

    void addCurrentOffset(uint64_t value) {
        uint64_t* newOffsets = nullptr;
        try {
            newOffsets = reinterpret_cast<uint64_t*>(gPool.allocate((currentOffsetSize + 1) * sizeof(uint64_t), alignof(uint64_t)));
        }
        catch (const std::bad_alloc& e) {
            std::cerr << "Memory allocation failed: " << e.what() << std::endl;
            return;
        }

        for (uint8_t i = 0; i < currentOffsetSize; ++i) {
            newOffsets[i] = currentOffsets ? reinterpret_cast<uint64_t*>(currentOffsets)[i] : 0;
        }

        if (currentOffsetSize > 0)
        {
            gPool.deallocate(currentOffsets, currentOffsetSize * sizeof(uint64_t));
        }

        newOffsets[currentOffsetSize] = value;
        currentOffsets = newOffsets;
        currentOffsetSize += 1;
    }

    void addPrevOffset(uint64_t value) {
        uint64_t* newOffsets = nullptr;
        try {
            newOffsets = reinterpret_cast<uint64_t*>(gPool.allocate((previousOffsetSize + 1) * sizeof(uint64_t), alignof(uint64_t)));
        }
        catch (const std::bad_alloc& e) {
            std::cerr << "Memory allocation failed: " << e.what() << std::endl;
            return;
        }

        for (uint8_t i = 0; i < previousOffsetSize; ++i) {
            newOffsets[i] = previousOffsets ? reinterpret_cast<uint64_t*>(previousOffsets)[i] : 0;
        }

        if (previousOffsetSize > 0)
        {
            gPool.deallocate(previousOffsets, previousOffsetSize * sizeof(uint64_t));
        }

        newOffsets[previousOffsetSize] = value;
        previousOffsets = newOffsets;
        previousOffsetSize += 1;
    }

    Node1** children() {
        return reinterpret_cast<Node1**>(childNodes);
    }
};

size_t buildTree(Node1* node, int depth) {
    if (withName) node->setName();
    if (withOffsets) node->allocateOffsets();

    nodeCount++;
    if (depth == 0) return 1;
    node->allocateChildren();
    for (int i = 0; i < fanout; ++i) {
        Node1* child = new (gPool.allocate(sizeof(Node1), alignof(Node1))) Node1();
        node->children()[i] = child;
        buildTree(child, depth - 1);
    }
    return nodeCount;
}

void freeTree(Node1* /*node*/) {
    // No-op: memory is released by resetting or destroying the pool
}

void testCase(const std::string& label, int fanout, int depth, bool withname, bool withoffsets) {
    std::cout << label << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    withName = withname;
    withOffsets = withoffsets;
    nodeCount = 0;
    gPool.reset();

    Node1* root = new (gPool.allocate(sizeof(Node1), alignof(Node1))) Node1();
    buildTree(root, depth);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Created " << nodeCount << " nodes in " << duration.count() << " seconds.\n" << ":current used memory is :" << gPool.getOffset() / (1024 * 1024) << " MB";

    // No need to freeTree(root); memory is managed by the pool
}

void testCase1(size_t totalNodes, size_t maxChildren) {
    std::string strConstNodeName = "1234567890";
    gPool.reset();

    Node1 root;
    root.setName(strConstNodeName);
    root.addCurrentOffset(123);
    root.addCurrentOffset(123);
    root.addPrevOffset(123);
    root.addPrevOffset(123);

    std::queue<Node1*> q;
    q.push(&root);

    size_t createdNodes = 1; // root counts as 1

    auto start = std::chrono::high_resolution_clock::now();

    while (createdNodes < totalNodes) {
        Node1* current = q.front();
        q.pop();

        size_t childrenToCreate = std::min(maxChildren, totalNodes - createdNodes);
        for (size_t i = 0; i < childrenToCreate; ++i) {
            Node1* newNode = current->AddAChild(strConstNodeName);
            newNode->addCurrentOffset(123);
            newNode->addCurrentOffset(234);
            newNode->addPrevOffset(567);
            newNode->addPrevOffset(678);
            q.push(newNode);

            ++createdNodes;
            if (createdNodes >= totalNodes) break;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Created " << createdNodes << " nodes in " << duration.count() << " seconds.\n"
        << "Current used memory: " << gPool.getOffset() / (1024 * 1024) << " MB\n";
}


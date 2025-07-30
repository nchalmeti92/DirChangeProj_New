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
#include <fstream>
#include <algorithm>
#include <vector>
#include <unordered_set>

// Simple bump-pointer memory pool
class MemoryPool {
public:
    void reset();
    static MemoryPool* createInstance();

    void* allocate(size_t size, size_t alignment = alignof(std::max_align_t));

    void deallocate(void* ptr, size_t size);
    size_t getOffset() const;

private:
    MemoryPool(size_t size) : poolSize(size), offset(0) {
        pool = (char*)std::malloc(poolSize);
        if (!pool) throw std::bad_alloc();
    }
    ~MemoryPool() {
        std::free(pool);
    }
    MemoryPool(const MemoryPool&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;

    char* pool;
    size_t poolSize;
    size_t offset;
    std::unordered_map<size_t, std::vector<void*>> freeChunkPoolMemory;

    static MemoryPool* poolInstance;
};

static const std::string fixedName = "abcdefghij"; // 10 chars
static const int offSetCount = 2;
static const int fanout = 10;
static bool withName = false;
static bool withOffsets = false;
static int nodeCount = 0;
static MemoryPool* gPool = nullptr;

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
        if (gPool == nullptr) {
            if (currentOffsets) {
                free(currentOffsets);
                currentOffsets = nullptr;
            }  
            if (previousOffsets)
            {
                free(previousOffsets);
                previousOffsets = nullptr;
            }
            if (childNodes)
            {
                free(childNodes);
                childNodes = nullptr;
            }
            if (name)
            {
                delete[] name;
                name = nullptr;
            }
        }
    }

    void setName() {
        name = (gPool != nullptr) ? (char*)gPool->allocate(fixedName.size() + 1, alignof(char)) : new char[fixedName.size() + 1];
        strcpy_s(name, fixedName.size() + 1, fixedName.c_str());
    }

    void setName(const std::string& inputname) {
        name = (gPool != nullptr) ? (char*)gPool->allocate(inputname.size() + 1, alignof(char)) : new char[inputname.size() + 1];
        strcpy_s(name, inputname.size() + 1, inputname.c_str());
    }

    void allocateOffsets() {
        currentOffsetSize = offSetCount;
        previousOffsetSize = offSetCount;
        if (offSetCount > 0) {
            currentOffsets = (gPool != nullptr) ? gPool->allocate(offSetCount * sizeof(uint64_t), alignof(uint64_t)) : malloc(offSetCount * sizeof(uint64_t));
            std::memset(currentOffsets, 0, offSetCount * sizeof(uint64_t));
        }
        if (offSetCount > 0) {
            previousOffsets = (gPool != nullptr) ? gPool->allocate(offSetCount * sizeof(uint64_t), alignof(uint64_t)) : malloc(offSetCount * sizeof(uint64_t));
            std::memset(previousOffsets, 0, offSetCount * sizeof(uint64_t));
        }
    }

    void allocateChildren() {
        childCount = fanout;
        if (fanout > 0) {
            childNodes = (gPool != nullptr) ? gPool->allocate(fanout * sizeof(Node1*), alignof(Node1*)) : new Node1[fanout * sizeof(Node1*)];
            std::memset(childNodes, 0, fanout * sizeof(Node1*));
        }
    }

    Node1* AddAChild(const std::string& name) {
        Node1* child = (gPool != nullptr) ? new (gPool->allocate(sizeof(Node1), alignof(Node1))) Node1() : new Node1();
        
        addChild(child);
        child->setName(name);
        return child;
    }

    /*void addChild(Node1* child) {
        Node1** newChildren = nullptr;
        try {
            newChildren = reinterpret_cast<Node1**>((gPool != nullptr) ? gPool->allocate((childCount + 1) * sizeof(Node1*), alignof(Node1*)) : );
        }
        catch (const std::bad_alloc& e) {
            std::cerr << "Memory allocation failed: " << e.what() << std::endl;
            return;
        }

        if (childCount > 0)
        {
            for (uint32_t i = 0; i < childCount; ++i) {
                newChildren[i] = children()[i];
            }

            (gPool != nullptr) ? gPool->deallocate(childNodes, childCount * sizeof(Node1*));
        }

        newChildren[childCount] = child;
        childNodes = newChildren;
        childCount += 1;
    }*/

    void addChild(Node1* child) {
        Node1** newChildren = nullptr;
        size_t allocSize = (childCount + 1) * sizeof(Node1*);
        
        // Allocate new children array
        if (gPool != nullptr) {
            try {
                newChildren = reinterpret_cast<Node1**>(gPool->allocate(allocSize, alignof(Node1*)));
            }
            catch (const std::bad_alloc& e) {
                std::cerr << "Memory allocation failed: " << e.what() << std::endl;
                return;
            }
        }
        else {
            newChildren = reinterpret_cast<Node1**>(malloc(allocSize));
            if (!newChildren) {
                std::cerr << "Memory allocation failed (malloc)" << std::endl;
                return;
            }
        }

        // Copy existing children
        for (uint32_t i = 0; i < childCount; ++i) {
            newChildren[i] = children()[i];
        }

        // Free old children array if needed
        if (childCount > 0) {
            if (gPool != nullptr) {
                gPool->deallocate(childNodes, childCount * sizeof(Node1*));
            }
            else {
                free(childNodes);
            }
        }

        newChildren[childCount] = child;
        childNodes = newChildren;
        childCount += 1;
    }

    /*void addCurrentOffset(uint64_t value) {
        uint64_t* newOffsets = nullptr;
        try {
            newOffsets = reinterpret_cast<uint64_t*>((gPool != nullptr) ? gPool->allocate((currentOffsetSize + 1) * sizeof(uint64_t), alignof(uint64_t)));
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
            (gPool != nullptr) ? gPool->deallocate(currentOffsets, currentOffsetSize * sizeof(uint64_t)) : free(currentOffsets);
        }

        newOffsets[currentOffsetSize] = value;
        currentOffsets = newOffsets;
        currentOffsetSize += 1;
    }*/
    
    void addCurrentOffset(uint64_t value) {
        uint64_t* newOffsets = nullptr;
        try {
            if (gPool != nullptr) {
                newOffsets = reinterpret_cast<uint64_t*>(gPool->allocate((currentOffsetSize + 1) * sizeof(uint64_t), alignof(uint64_t)));
            }
            else {
                newOffsets = reinterpret_cast<uint64_t*>(malloc((currentOffsetSize + 1) * sizeof(uint64_t)));
                if (!newOffsets) {
                    std::cerr << "Memory allocation failed (malloc)" << std::endl;
                    return;
                }
            }
        }
        catch (const std::bad_alloc& e) {
            std::cerr << "Memory allocation failed: " << e.what() << std::endl;
            return;
        }

        for (uint8_t i = 0; i < currentOffsetSize; ++i) {
            newOffsets[i] = currentOffsets ? reinterpret_cast<uint64_t*>(currentOffsets)[i] : 0;
        }

        if (currentOffsetSize > 0) {
            if (gPool != nullptr) {
                gPool->deallocate(currentOffsets, currentOffsetSize * sizeof(uint64_t));
            }
            else {
                free(currentOffsets);
            }
        }

        newOffsets[currentOffsetSize] = value;
        currentOffsets = newOffsets;
        currentOffsetSize += 1;
    }

    /*void addPrevOffset(uint64_t value) {
        uint64_t* newOffsets = nullptr;
        try {
            newOffsets = reinterpret_cast<uint64_t*>((gPool != nullptr) ? gPool->allocate((previousOffsetSize + 1) * sizeof(uint64_t), alignof(uint64_t)) : );
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
            (gPool != nullptr) ? gPool->deallocate(previousOffsets, previousOffsetSize * sizeof(uint64_t)) : ;
        }

        newOffsets[previousOffsetSize] = value;
        previousOffsets = newOffsets;
        previousOffsetSize += 1;
    }*/

    void addPrevOffset(uint64_t value) {
        uint64_t* newOffsets = nullptr;
        try {
            if (gPool != nullptr) {
                newOffsets = reinterpret_cast<uint64_t*>(gPool->allocate((previousOffsetSize + 1) * sizeof(uint64_t), alignof(uint64_t)));
            }
            else {
                newOffsets = reinterpret_cast<uint64_t*>(malloc((previousOffsetSize + 1) * sizeof(uint64_t)));
                if (!newOffsets) {
                    std::cerr << "Memory allocation failed (malloc)" << std::endl;
                    return;
                }
            }
        }
        catch (const std::bad_alloc& e) {
            std::cerr << "Memory allocation failed: " << e.what() << std::endl;
            return;
        }

        for (uint8_t i = 0; i < previousOffsetSize; ++i) {
            newOffsets[i] = previousOffsets ? reinterpret_cast<uint64_t*>(previousOffsets)[i] : 0;
        }

        if (previousOffsetSize > 0) {
            if (gPool != nullptr) {
                gPool->deallocate(previousOffsets, previousOffsetSize * sizeof(uint64_t));
            }
            else {
                free(previousOffsets);
            }
        }

        newOffsets[previousOffsetSize] = value;
        previousOffsets = newOffsets;
        previousOffsetSize += 1;
    }

    Node1** children() {
        return reinterpret_cast<Node1**>(childNodes);
    }
};


class FileView
{
public:

    size_t buildTree(Node1* node, int depth) {
        if (withName) node->setName();
        if (withOffsets) node->allocateOffsets();

        nodeCount++;
        if (depth == 0) return 1;
        node->allocateChildren();
        for (int i = 0; i < fanout; ++i) {
            Node1* child = (gPool != nullptr) ? (new (gPool->allocate(sizeof(Node1), alignof(Node1))) Node1()) : new Node1();
            node->children()[i] = child;
            buildTree(child, depth - 1);
        }
        return nodeCount;
    }

    void freeTree(Node1* /*node*/) {
        // No-op: memory is released by resetting or destroying the pool
    }

    void testCase(const std::string& label, int fanout, int depth, bool withname, bool withoffsets, bool useMemoryPool) {
        std::cout << label << std::endl;
        auto start = std::chrono::high_resolution_clock::now();

        withName = withname;
        withOffsets = withoffsets;
        nodeCount = 0;
        if (useMemoryPool) {
            gPool = MemoryPool::createInstance();
            gPool->reset();
        }
        else {
            gPool = nullptr; // Use default allocation
        }

        Node1* root = nullptr;
        (gPool != nullptr) ? new (gPool->allocate(sizeof(Node1), alignof(Node1))) Node1() : new Node1();
        buildTree(root, depth);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        //std::cout << "Created " << nodeCount << " nodes in " << duration.count() << " seconds.\n" << ":current used memory is :" << gPool.getOffset() / (1024 * 1024) << " MB";

        // No need to freeTree(root); memory is managed by the pool
    }

    void testCase1(size_t totalNodes, size_t maxChildren, bool useMemoryPool) {
        if (useMemoryPool) {
            gPool = MemoryPool::createInstance();
            gPool->reset();
        }
        else {
            gPool = nullptr; // Use default allocation
        }

        std::string strConstNodeName = "1234567890";

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
                newNode->addPrevOffset(123);
                newNode->addPrevOffset(678);
                q.push(newNode);

                ++createdNodes;
                if (createdNodes >= totalNodes) break;
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::cout << "Created " << createdNodes << " nodes in " << duration.count() << " seconds.\n"
            << "Current used memory: " << ((gPool != nullptr) ? gPool->getOffset() : 0) / (1024 * 1024) << " MB\n";

		checkNodeOffsetsForTest(&root);
    }

    // Helper: Read a line from file at a given offset
    std::string readLineAtOffset(std::ifstream& file, uint64_t offset) {
        file.clear();
        file.seekg(offset, std::ios::beg);
        std::string line;
        std::getline(file, line);
        return line;
    }

    
    bool checkNodeOffsetsForTest(Node1* node) {
		if (!node) {
			return false; // Invalid node or file
		}
        uint64_t* curr = reinterpret_cast<uint64_t*>(node->currentOffsets);
        uint64_t* prev = reinterpret_cast<uint64_t*>(node->previousOffsets);

        std::unordered_set<uint64_t> currSet;
        std::unordered_set<uint64_t> prevSet;

        // Read lines at currentOffsets and insert into currSet
        for (uint8_t i = 0; i < node->currentOffsetSize; ++i) {
            currSet.insert(curr[i]);
        }

        // Read lines at previousOffsets and insert into prevSet
        for (uint8_t j = 0; j < node->previousOffsetSize; ++j) {
            prevSet.insert(prev[j]);
        }

        // Print "new" for values in currSet not in prevSet
        for (const auto& val : currSet) {
            if (!prevSet.contains(val)) {
                std::cout << "new: " << val << std::endl;
            }
        }

        // Print "deleted" for values in prevSet not in currSet
        for (const auto& val : prevSet) {
            if (!currSet.contains(val)) {
                std::cout << "deleted: " << val << std::endl;
            }
        }

        // Recursively check children
        for (uint32_t i = 0; i < node->childCount; ++i) {
            if (!checkNodeOffsetsForTest(node->children()[i])) {
                return false;
            }
        }
        return true;
    }
    
    // Check if all currentOffsets are present in previousOffsets and compare file lines
    bool CompareFiles(Node1* node, std::ifstream& file) {

		if (!node || !file.is_open()) {
			return false; // Invalid node or file
		}

        uint64_t* curr = reinterpret_cast<uint64_t*>(node->currentOffsets);
        uint64_t* prev = reinterpret_cast<uint64_t*>(node->previousOffsets);

        std::unordered_set<std::string> currSet;
        std::unordered_set<std::string> prevSet;

        // Read lines at currentOffsets and insert into currSet
        for (uint8_t i = 0; i < node->currentOffsetSize; ++i) {
            std::string line = readLineAtOffset(file, curr[i]);
            currSet.insert(line);
        }

        // Read lines at previousOffsets and insert into prevSet
        for (uint8_t j = 0; j < node->previousOffsetSize; ++j) {
            std::string line = readLineAtOffset(file, prev[j]);
            prevSet.insert(line);
        }

        // Print "new" for values in currSet not in prevSet
        for (const auto& val : currSet) {
            if (!prevSet.contains(val)) {
                std::cout << "new: " << val << std::endl;
            }
        }

        // Print "deleted" for values in prevSet not in currSet
        for (const auto& val : prevSet) {
            if (!currSet.contains(val)) {
                std::cout << "deleted: " << val << std::endl;
            }
        }

        // Recursively check children
        for (uint32_t i = 0; i < node->childCount; ++i) {
            if (!CompareFiles(node->children()[i], file)) {
                return false;
            }
        }
        return true;
    }

};


#pragma once
#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>
#include <queue>
#include <string>
#include <iostream>
#include <cstring>
#include <chrono>
#include <cmath>
#include <cassert>

struct Node2; // Forward declaration

// Pools for offsets and children
static boost::pool<> offsetPool(sizeof(uint64_t));
static boost::pool<> childPool(sizeof(Node2*));

// Node2 structure with boost memory pool
struct Node2 {
    void* currentOffsets;
    void* previousOffsets;
    void* childNodes;
    char* name;

    uint32_t childCount;
    uint8_t currentOffsetSize;
    uint8_t previousOffsetSize;
    uint16_t flags = 0;

    Node2() : currentOffsets(nullptr), previousOffsets(nullptr), childNodes(nullptr),
        name(nullptr), childCount(0), currentOffsetSize(0), previousOffsetSize(0) {
    }

    ~Node2() {
        if (currentOffsets) offsetPool.free(currentOffsets);
        if (previousOffsets) offsetPool.free(previousOffsets);
        if (childNodes) childPool.free(childNodes);
        if (name) std::free(name);
    }

    void setName(const std::string& inputname) {
        name = (char*)std::malloc(inputname.size() + 1);
        strcpy_s(name, inputname.size() + 1, inputname.c_str());
    }

    void allocateOffsets() {
        currentOffsetSize = 2;
        previousOffsetSize = 2;
        currentOffsets = offsetPool.ordered_malloc(2); // Allocates 2 blocks (16 bytes)
        std::memset(currentOffsets, 0, 2 * sizeof(uint64_t));
        previousOffsets = offsetPool.ordered_malloc(2);
        std::memset(previousOffsets, 0, 2 * sizeof(uint64_t));
    }

    void allocateChildren() {
        childCount = 10;
        childNodes = childPool.ordered_malloc(10); // Allocates 10 blocks (10 * sizeof(Node2*))
        std::memset(childNodes, 0, 10 * sizeof(Node2*));
    }

    void addChild(Node2* child) {
        Node2** newChildren = reinterpret_cast<Node2**>(childPool.ordered_malloc(childCount + 1));
        for (uint32_t i = 0; i < childCount; ++i) {
            newChildren[i] = children()[i];
        }
        newChildren[childCount] = child;
        if (childNodes) childPool.free(childNodes);
        childNodes = newChildren;
        childCount += 1;
    }

    void addCurrentOffset(uint64_t value) {
        uint64_t* newOffsets = reinterpret_cast<uint64_t*>(offsetPool.ordered_malloc(currentOffsetSize + 1));
        for (uint8_t i = 0; i < currentOffsetSize; ++i) {
            newOffsets[i] = currentOffsets ? reinterpret_cast<uint64_t*>(currentOffsets)[i] : 0;
        }
        newOffsets[currentOffsetSize] = value;
        if (currentOffsets) offsetPool.free(currentOffsets);
        currentOffsets = newOffsets;
        currentOffsetSize += 1;
    }

    void addPrevOffset(uint64_t value) {
        uint64_t* newOffsets = reinterpret_cast<uint64_t*>(offsetPool.ordered_malloc(previousOffsetSize + 1));
        for (uint8_t i = 0; i < previousOffsetSize; ++i) {
            newOffsets[i] = previousOffsets ? reinterpret_cast<uint64_t*>(previousOffsets)[i] : 0;
        }
        newOffsets[previousOffsetSize] = value;
        if (previousOffsets) offsetPool.free(previousOffsets);
        previousOffsets = newOffsets;
        previousOffsetSize += 1;
    }

    Node2** children() {
        return reinterpret_cast<Node2**>(childNodes);
    }
};


void testCaseForNode2(size_t totalNodes, size_t maxChildren) {
    std::string strConstNodeName = "1234567890";
    boost::object_pool<Node2> nodePool; // Use Boost object pool for Node2 allocation

    Node2* root = nodePool.construct();
    root->setName(strConstNodeName);
    root->addCurrentOffset(123);
    root->addCurrentOffset(123);
    root->addPrevOffset(123);
    root->addPrevOffset(123);

    std::queue<Node2*> q;
    q.push(root);

    size_t createdNodes = 1; // root counts as 1

    auto start = std::chrono::high_resolution_clock::now();

    while (createdNodes < totalNodes) {
        Node2* current = q.front();
        q.pop();

        size_t childrenToCreate = std::min(maxChildren, totalNodes - createdNodes);
        for (size_t i = 0; i < childrenToCreate; ++i) {
            Node2* newNode = nodePool.construct();
            current->addChild(newNode);
            newNode->setName(strConstNodeName);
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
    std::cout << "Created " << createdNodes << " nodes in " << duration.count() << " seconds.\n";
    // Memory usage estimation
    size_t offsetPoolMemory = offsetPool.get_next_size() * sizeof(uint64_t);
    size_t childPoolMemory = childPool.get_next_size() * sizeof(Node2*);
    std::cout << "Estimated memory usage: "
        << (offsetPoolMemory + childPoolMemory) / (1024.0 * 1024.0)
        << " MB\n";

    offsetPool.release_memory();
    childPool.release_memory();
}
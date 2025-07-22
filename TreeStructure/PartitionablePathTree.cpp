// PartitionablePathTree.cpp
// A minimal C++ prototype to build a path tree and split it into balanced subtrees
// Target: ≤ 50 bytes per node

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <sstream>
#include <algorithm>

constexpr int MAX_NAME_LENGTH = 32; // Limit name to 32 bytes
constexpr int MAX_CHILDREN = 64;    // Hard cap to avoid bloat

// Minimal node structure (~48 bytes per node expected)
struct Node {
    char name[MAX_NAME_LENGTH];                // 32 bytes
    std::vector<std::shared_ptr<Node>> children; // ~8–16 bytes overhead (pointer + small vector)
    Node* parent = nullptr;                    // 8 bytes
    int childCount = 0;                        // 4 bytes

    Node(const std::string& n, Node* p = nullptr) : parent(p) {
        std::snprintf(name, MAX_NAME_LENGTH, "%s", n.c_str());
    }
};

using NodePtr = std::shared_ptr<Node>;

NodePtr insertPath(NodePtr root, const std::string& path) {
    std::istringstream ss(path);
    std::string token;
    NodePtr current = root;

    while (std::getline(ss, token, '/')) {
        if (token.empty()) continue;

        auto it = std::find_if(current->children.begin(), current->children.end(),
            [&token](const NodePtr& child) { return std::string(child->name) == token; });

        if (it != current->children.end()) {
            current = *it;
        } else {
            NodePtr newNode = std::make_shared<Node>(token, current.get());
            current->children.push_back(newNode);
            current->childCount++;
            current = newNode;
        }
    }
    return current;
}

// Post-order partitioning
int splitTree(NodePtr node, int maxNodes, std::vector<NodePtr>& subRoots) {
    int total = 1;
    for (auto& child : node->children) {
        total += splitTree(child, maxNodes, subRoots);
    }

    if (total >= maxNodes && node->parent != nullptr) {
        subRoots.push_back(node);
        node->parent->children.erase(
            std::remove_if(node->parent->children.begin(), node->parent->children.end(),
                [&node](const NodePtr& n) { return n.get() == node.get(); }),
            node->parent->children.end());
        return 0; // Cut here
    }
    return total;
}

void printTree(NodePtr node, int depth = 0) {
    std::cout << std::string(depth * 2, ' ') << node->name << "\n";
    for (auto& child : node->children) {
        printTree(child, depth + 1);
    }
}

int main() {
    NodePtr root = std::make_shared<Node>("");

    // Simulate input
    std::vector<std::string> paths = {
        "a/b/c/d1", "a/b/c/d2", "a/b/x/y/z", "a/b/x/y/w",
        "m/n/o", "m/n/p", "m/q/r/s", "m/q/r/t"
    };

    for (const auto& path : paths) {
        insertPath(root, path);
    }

    std::vector<NodePtr> subTrees;
    splitTree(root, 3, subTrees);

    std::cout << "Root Tree:\n";
    printTree(root);

    std::cout << "\nSubtrees:\n";
    for (auto& sub : subTrees) {
        std::cout << "Subtree Root: " << sub->name << "\n";
        printTree(sub);
        std::cout << "--------\n";
    }

    return 0;
}

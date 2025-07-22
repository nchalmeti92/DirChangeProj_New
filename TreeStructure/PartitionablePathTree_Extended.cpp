#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cstring>

constexpr int MAX_NAME_LENGTH = 32;

struct Node {
    char name[MAX_NAME_LENGTH];
    std::vector<std::shared_ptr<Node>> children;
    Node* parent = nullptr;
    //int childCount = 0;

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
        }
        else {
            NodePtr newNode = std::make_shared<Node>(token, current.get());
            current->children.push_back(newNode);
            //current->childCount++;
            current = newNode;
        }
    }
    return current;
}

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
        return 0;
    }
    return total;
}

void printTree(NodePtr node, std::ostream& out, int depth = 0) {
    out << std::string(depth * 2, ' ') << node->name << "\n";
    for (auto& child : node->children) {
        printTree(child, out, depth + 1);
    }
}

int countNodes(NodePtr node) {
    int total = 1;
    for (auto& child : node->children) {
        total += countNodes(child);
    }
    return total;
}

void serializeSubtrees(const std::vector<NodePtr>& subRoots) {
    int index = 1;
    for (const auto& sub : subRoots) {
        std::ofstream out("subtree_" + std::to_string(index++) + ".txt");
        printTree(sub, out);
        out.close();
    }
}

std::vector<std::string> readPathsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> paths;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) paths.push_back(line);
    }
    return paths;
}

std::vector<std::string> generateFolderPaths(int rootCount, int depth, int subfoldersPerLevel) {
    std::vector<std::string> paths;

    std::function<void(std::string, int)> recurse = [&](std::string prefix, int currentDepth) {
        if (currentDepth > depth) {
            paths.push_back(prefix);
            return;
        }

        for (int i = 1; i <= subfoldersPerLevel; ++i) {
            std::ostringstream oss;
            oss << prefix << "/folder_" << currentDepth << "_" << i;
            recurse(oss.str(), currentDepth + 1);
        }
        };

    for (int i = 1; i <= rootCount; ++i) {
        std::ostringstream root;
        root << "/root_" << i;
        recurse(root.str(), 1);
    }

    return paths;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <input_path_file.txt> <max_nodes_per_subtree>\n";
        return 1;
    }

    std::string inputFile = argv[1];
    int maxNodes = std::stoi(argv[2]);

    std::vector<std::string> paths = generateFolderPaths(1, 6, 10); // readPathsFromFile(inputFile);
    NodePtr root = std::make_shared<Node>("");

    for (const auto& path : paths) {
        insertPath(root, path);
    }

    std::vector<NodePtr> subTrees;
    splitTree(root, maxNodes, subTrees);

    std::cout << "\n=== Root Tree ===\n";
    printTree(root, std::cout);

    std::cout << "\n=== Subtrees ===\n";
    serializeSubtrees(subTrees);
    int idx = 1;
    for (const auto& sub : subTrees) {
        std::cout << "Subtree_" << idx << " nodes: " << countNodes(sub) << "\n";
        ++idx;
    }

    int total = countNodes(root);
    for (const auto& sub : subTrees) {
        total += countNodes(sub);
    }

    std::cout << "\n[INFO] Total nodes in all trees: " << total << "\n";
    std::cout << "[INFO] Approx memory usage: " << total * 48 / 1024.0 << " KB\n";
    return 0;
}
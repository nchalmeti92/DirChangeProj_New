//// TreeStructure.cpp : This file contains the 'main' function. Program execution begins and ends there.
////
//
////#include <iostream>
////
////int main()
////{
////    std::cout << "Hello World!\n";
////}
//
//// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
//// Debug program: F5 or Debug > Start Debugging menu
//
//// Tips for Getting Started: 
////   1. Use the Solution Explorer window to add/manage files
////   2. Use the Team Explorer window to connect to source control
////   3. Use the Output window to see build output and other messages
////   4. Use the Error List window to view errors
////   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
////   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
//
//
////#include <iostream>
////#include <string>
////#include <list>
////#include <array>
////#include <memory>
////#include <sstream>
////#include <thread>
////#include <chrono>
////
////struct Node {
////    char name[15];
////    std::list<Node*>* children;
////    std::array<uint8_t, 10> flags;
////
////    Node(const std::string& n)
////        : name(n), children(new std::list<Node*>), flags{ 0 } {
////    }
////
////    ~Node() {
////        for (auto child : *children) {
////            delete child;
////        }
////        delete children;
////    }
////};
////
////// Function to insert a single child node
////void InsertChild(Node* parent, const std::string& childName) {
////    Node* child = new Node(childName);
////    parent->children->push_back(child);
////}
////
////// Function to insert multiple children with configurable count
////void InsertMultipleChildren(Node* parent, int count, const std::string& baseName) {
////    for (int i = 1; i <= count; ++i) {
////        std::ostringstream oss;
////        oss << baseName << i;
////        InsertChild(parent, oss.str());
////    }
////}
////
////// Recursive function to add subfolders to each node
////void AddSubfolders(Node* node, int subfolderCount, int depth, int maxDepth) {
////    if (depth > maxDepth) return;
////    InsertMultipleChildren(node, subfolderCount, "subfolder_");
////    for (auto child : *node->children) {
////        AddSubfolders(child, subfolderCount, depth + 1, maxDepth);
////    }
////}
////
////// Recursive function to print the tree
////void PrintTree(const Node* node, int depth = 0) {
////    for (int i = 0; i < depth; ++i) std::cout << "  ";
////    std::cout << node->name << std::endl;
////    for (const auto& child : *node->children) {
////        PrintTree(child, depth + 1);
////    }
////}
////
////size_t GetFolderCount(const Node* node) {
////    size_t count = 1; // Count this node
////    for (const auto& child : *node->children) {
////        count += GetFolderCount(child);
////    }
////    return count;
////} 
////
////size_t foldercountwithformula(int subfoldercount, int depth)
////{
////    if (subfoldercount == 1) {
////        // Special case: a degenerate tree (a line)
////        return depth + 1;
////    }
////    size_t nodes = (static_cast<size_t>(std::pow(subfoldercount, depth + 1)) - 1) / (subfoldercount - 1);
////
////    std::cout << "Returning :" << nodes << std::endl;
////    return nodes;
////}
////
////int main()
////{
////    int subfolderCount = 10; // Configurable subfolder count
////    int treeDepth = 6;       // Configurable tree depth
////
////    Node* root = new Node("root");
////    AddSubfolders(root, subfolderCount, 1, treeDepth);
////
////
////    std::cout << "Total folder count: " << ":" << foldercountwithformula(subfolderCount, treeDepth) <<":size of node:"<<sizeof(Node) << std::endl;
////
////    //PrintTree(root);
////    std::this_thread::sleep_for(std::chrono::seconds(100000));
////    delete root;
////    return 0;
////}
//
//
//
//
//
//#include <iostream>
//#include <string>
//#include <vector>
//#include <array>
//#include <memory>
//#include <sstream>
//#include <thread>
//#include <chrono>
//#include <cstring>
//#include <cmath>
////
////
////struct Node { // 40 bytes
////    //std::string name;
////	char name[13]; // 15 bytes, null-terminated
////    std::vector<Node> children;
////    std::array<uint8_t, 10> flags;
////
////    Node(const std::string& n)
////        :  /*name(n),*/ flags{ 0 } { // taking 16 bytes
////        // Copy up to 14 characters, ensure null-termination
////        strncpy_s(name, sizeof(name), n.c_str(), sizeof(name) - 1);
////        name[n.length()] = '\0';
////    }
////
////    ~Node() {
////        /*for (auto child : children) {
////            delete child;
////        }*/
////        //delete children;
////    }
////};
////
////// Function to insert a single child node
//////void InsertChild(Node* parent, const std::string& childName) {
//////    //Node* child = new Node(childName);
//////    parent->children.emplace_back(childName);
//////}
//////
//////// Function to insert multiple children with configurable count
//////void InsertMultipleChildren(Node* parent, int count, const std::string& baseName) {
//////    for (int i = 1; i <= count; ++i) {
//////        std::ostringstream oss;
//////        oss << baseName << i;
//////        InsertChild(parent, oss.str());
//////    }
//////}
////
////// Recursive function to add subfolders to each node
//////void AddSubfolders(Node* node, int subfolderCount, int depth, int maxDepth) {
//////    if (depth == maxDepth) return;
//////    InsertMultipleChildren(node, subfolderCount, "subfolder_");
//////    for (auto child : node->children) {
//////        AddSubfolders(child, subfolderCount, depth + 1, maxDepth);
//////    }
//////}
////
////void AddSubfolders1(Node* node, int subfolderCount, int depth, int maxDepth) {
////    if (depth == maxDepth) return;
////    for (int i = 1; i <= subfolderCount; ++i) {
////        std::ostringstream oss;
////        oss << "subfolder_" << i;
////        //node->children.emplace_back(oss.str());
////        Node* nd = new Node(oss.str());
////        node->children.push_back(nd);
////        AddSubfolders1(node->children.back(), subfolderCount, depth + 1, maxDepth);
////    }
////}
////
////// Recursive function to print the tree
////void PrintTree(const Node* node, int depth = 0) {
////    for (int i = 0; i < depth; ++i) std::cout << "  ";
////    std::cout << node->name << std::endl;
////    for (const auto& child : node->children) {
////        PrintTree(child, depth + 1);
////    }
////}
////
////size_t GetFolderCount(const Node* node) {
////    size_t count = 1; // Count this node
////    for (const auto& child : node->children) {
////        count += GetFolderCount(child);
////    }
////    return count;
////}
////
////size_t foldercountwithformula(int subfoldercount, int depth) {
////    if (subfoldercount == 1) {
////        // Special case: a degenerate tree (a line)
////        return depth + 1;
////    }
////    size_t nodes = (static_cast<size_t>(std::pow(subfoldercount, depth + 1)) - 1) / (subfoldercount - 1);
////    std::cout << "Returning :" << nodes << std::endl;
////    return nodes;
////}
////
////int main() {
////    int subfolderCount = 10; // Configurable subfolder count
////    int treeDepth = 6;       // Configurable tree depth
////
////    //Node* root = new Node("root");
////	Node root("root"); // Using stack allocation for root node
////    AddSubfolders1(&root, subfolderCount, 0, treeDepth);
////
////    std::cout << "Total folder count: :" << foldercountwithformula(subfolderCount, treeDepth)
////        << ":size of node:" << sizeof(Node) << std::endl;
////
////    //PrintTree(root);
////    //std::this_thread::sleep_for(std::chrono::seconds(100000));
////    //delete root;
////    return 0;
////}
//
//#include <memory>
//
//
//struct Node { // 40 bytes
//	//char name[13]; // 13 bytes, null-terminated using this node size is 56 bytes and total used memory for 1010101 nodes is 56 MB
//	std::string name; // taking up 29 bytes, null-terminated using this node size is 72 bytes and total used memory for 1010101 nodes is 74324 KB
//    std::vector<Node> children;
//    std::array<uint8_t, 10> flags;
//
//    Node(const std::string& n)
//        :  name(n), flags{ 0 } { // taking 16 bytes
//        // Copy up to 14 characters, ensure null-termination
//        /*strncpy_s(name, sizeof(name), n.c_str(), sizeof(name) - 1);
//        name[n.length()] = '\0';*/
//    }
//
//    ~Node() {
//        /*for (auto child : children) {
//            delete child;
//        }*/
//        //delete children;
//    }
//};
//
//// Function to insert a single child node
////void InsertChild(Node* parent, const std::string& childName) {
////    //Node* child = new Node(childName);
////    parent->children.emplace_back(childName);
////}
////
////// Function to insert multiple children with configurable count
////void InsertMultipleChildren(Node* parent, int count, const std::string& baseName) {
////    for (int i = 1; i <= count; ++i) {
////        std::ostringstream oss;
////        oss << baseName << i;
////        InsertChild(parent, oss.str());
////    }
////}
//
//// Recursive function to add subfolders to each node
////void AddSubfolders(Node* node, int subfolderCount, int depth, int maxDepth) {
////    if (depth == maxDepth) return;
////    InsertMultipleChildren(node, subfolderCount, "subfolder_");
////    for (auto child : node->children) {
////        AddSubfolders(child, subfolderCount, depth + 1, maxDepth);
////    }
////}
//
//void AddSubfolders1(Node* node, int subfolderCount, int depth, int maxDepth) {
//    if (depth == maxDepth) return;
//    for (int i = 1; i <= subfolderCount; ++i) {
//        std::ostringstream oss;
//        oss << "subfolder_" << i;
//        if (depth < maxDepth)
//        {
//            node->children.reserve(subfolderCount);
//        }
//        node->children.emplace_back(oss.str());
//       
//        AddSubfolders1(&node->children.back(), subfolderCount, depth + 1, maxDepth);
//    }
//}
//
//// Recursive function to print the tree
//void PrintTree(const Node* node, int depth = 0) {
//    for (int i = 0; i < depth; ++i) std::cout << "  ";
//    std::cout << node->name << std::endl;
//    for (const auto& child : node->children) {
//        PrintTree(&child, depth + 1);
//    }
//}
//
//size_t GetFolderCount(const Node* node) {
//    size_t count = 1; // Count this node
//    for (const auto& child : node->children) {
//        count += GetFolderCount(&child);
//    }
//    return count;
//}
//
//size_t foldercountwithformula(int subfoldercount, int depth) {
//    if (subfoldercount == 1) {
//        // Special case: a degenerate tree (a line)
//        return depth + 1;
//    }
//    size_t nodes = (static_cast<size_t>(std::pow(subfoldercount, depth + 1)) - 1) / (subfoldercount - 1);
//    std::cout << "Returning :" << nodes << std::endl;
//    return nodes;
//}
//
//int main() {
//    int subfolderCount = 100; // Configurable subfolder count
//    int treeDepth = 3;       // Configurable tree depth
//
//    //Node* root = new Node("root");
//    Node root("root"); // Using stack allocation for root node
//    AddSubfolders1(&root, subfolderCount, 0, treeDepth);
//
//    std::cout << "Total folder count: :" << foldercountwithformula(subfolderCount, treeDepth)
//        << ":size of node:" << sizeof(Node) << std::endl;
//
//    //PrintTree(root);
//    //std::this_thread::sleep_for(std::chrono::seconds(100000));
//    //delete root;
//    return 0;
//}




//==========================================================================
//                    Partitionable path tree
//========================================================================== =


//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <string>
//#include <memory>
//#include <sstream>
//#include <algorithm>
//#include <iomanip>
//#include <cstring>
//#include <array>
//#include <functional> // Add this include to resolve std::function
//
//constexpr int MAX_NAME_LENGTH = 32;
//
//struct Node {
//    std::string name;
//    //std::vector<std::shared_ptr<Node>> children; With this program is using 250 MB for 1111112 elements
//    std::vector<Node> children;
//    //Node* parent = nullptr;
//    //int childCount = 0;
//    bool reserved = false;
//    std::array<uint8_t, 10> flags;
//
//    Node(const std::string& n, Node* p = nullptr) : name(n)/*, parent(p)*/ {}
//};
//
////using NodePtr = std::shared_ptr<Node>;
//
//using NodePtr = Node*;
//
//NodePtr insertPath(NodePtr root, const std::string& path, int maxDepth, int subFolderCount) {
//    std::istringstream ss(path);
//    std::string token;
//    NodePtr current = root;
//
//    int depth = -3;
//    while (std::getline(ss, token, '/')) {
//        depth++;
//        if (token.empty()) continue;
//
//        auto it = std::find_if(current->children.begin(), current->children.end(),
//            [&token](const Node& child) { return std::string(child.name) == token; });
//
//        if (it != current->children.end()) {
//            current = &(*it);
//        }
//        else {
//            //NodePtr newNode = std::make_shared<Node>(token, current.get());
//            //current->children.push_back(newNode);
//            if (!current->reserved && depth < maxDepth)
//            {
//                current->children.reserve(subFolderCount);
//                current->reserved = true;
//            }
//            current->children.emplace_back(token);
//            //current->childCount++;
//            current = &current->children.back();
//        }
//    }
//    return current;
//}
//
////This is splitting only 500 subtrees accumulating 550000 nodes out of 1111112
////int splitTree(NodePtr node, int maxNodes, std::vector<NodePtr>& subRoots) {
////    if (!node) return 0; 
////    
////    int total = 1;
////    for (auto& child : node->children) {
////        total += splitTree(child, maxNodes, subRoots);
////    }
////
////    if (total >= maxNodes && node->parent != nullptr) {
////        subRoots.push_back(node);
////        node->parent->children.erase(
////            std::remove_if(node->parent->children.begin(), node->parent->children.end(),
////                [&node](const NodePtr& n) { return n.get() == node.get(); }),
////            node->parent->children.end());
////        return 0;
////    }
////    return total;
////}
//
//
//
//void printTree(NodePtr node, std::ostream& out, int depth = 0) {
//    out << std::string(depth * 2, ' ') << "├─" << node->name << "\n";
//    for (auto& child : node->children) {
//        printTree(&child, out, depth + 1);
//    }
//}
//
//int countNodes(NodePtr node) {
//    int total = 1;
//    for (auto& child : node->children) {
//        total += countNodes(&child);
//    }
//    return total;
//}
//
//void serializeSubtrees(const std::vector<NodePtr>& subRoots) {
//    static int index = 1;
//    for (const auto& sub : subRoots) {
//        std::ofstream out("subtree_" + std::to_string(index++) + ".txt");
//        printTree(sub, out);
//        out.close();
//    }
//}
//
//int splitTree(NodePtr node, int maxNodes, std::vector<NodePtr>& subRoots, NodePtr parent) {
//    if (!node) return 0;
//
//    int total = 1;
//
//    // Recurse children first
//    for (auto it = node->children.begin(); it != node->children.end(); ) {
//        int childTotal = splitTree(&(*it), maxNodes, subRoots, node);
//
//        // If child got split, it will be detached, so skip incrementing
//        if (childTotal == 0) {
//            it = node->children.erase(it);
//        }
//        else {
//            total += childTotal;
//            ++it;
//        }
//    }
//
//    // Now check this node itself
//    if (total >= maxNodes && parent != nullptr) {
//        std::vector<NodePtr> subRoots;
//        subRoots.push_back(node);
//        std::cout << "\n=== Subtrees ===\n";
//        serializeSubtrees(subRoots);
//        return 0; // Detach this node from above
//    }
//
//    return total;
//}
//
//std::vector<std::string> readPathsFromFile(const std::string& filename) {
//    std::ifstream file(filename);
//    std::vector<std::string> paths;
//    std::string line;
//    while (std::getline(file, line)) {
//        if (!line.empty()) paths.push_back(line);
//    }
//    return paths;
//}
//
//std::vector<std::string> generateFolderPaths(int rootCount, int depth, int subfoldersPerLevel) {
//    std::vector<std::string> paths;
//
//    std::function<void(std::string, int)> recurse = [&](std::string prefix, int currentDepth) {
//        if (currentDepth > depth) {
//            paths.push_back(prefix);
//            return;
//        }
//
//        for (int i = 1; i <= subfoldersPerLevel; ++i) {
//            std::ostringstream oss;
//            oss << prefix << "/subfolder_" << currentDepth << "_" << i;
//            recurse(oss.str(), currentDepth + 1);
//        }
//        };
//
//    for (int i = 1; i <= rootCount; ++i) {
//        std::ostringstream root;
//        root << "/folder_" << i;
//        recurse(root.str(), 1);
//    }
//
//    return paths;
//}
//
//int main(int argc, char* argv[]) {
//    /*if (argc < 2) {
//        std::cout << "Usage: " << argv[0] << " <input_path_file.txt> <max_nodes_per_subtree>\n";
//        return 1;
//    }*/
//
//    //std::string inputFile = argv[1];
//   // int maxNodes = std::stoi(argv[2]);
//	int maxNodes = 1000; // Default max nodes per subtree
//    int depth = 6;
//    int subFolderCount = 10;
//    //NodePtr root = std::make_shared<Node>("root");
//
//    Node root("root");
//    {
//        int count = 0;
//        std::vector<std::string> paths = generateFolderPaths(1, depth, subFolderCount); // readPathsFromFile(inputFile);
//        for (const auto& path : paths) {
//            insertPath(&root, path, depth, subFolderCount);
//            if (++count % 10000 == 0) {
//                std::cout << "[INFO] Inserted " << count << " paths...\n";
//            }
//        }
//    }
//
//    std::cout << "\n=== Root Tree ===\n";
//    //printTree(root, std::cout);
//
//    std::vector<NodePtr> subTrees;
//    splitTree(&root, maxNodes, subTrees, nullptr);
//
//    std::cout << "\n=== Subtrees ===\n";
//    if (subTrees.empty())
//    {
//		subTrees.push_back(&root); // If no subtrees were created, add the root itself   
//        serializeSubtrees(subTrees);
//    }
//   /* int idx = 1;
//    for (const auto& sub : subTrees) {
//        std::cout << "Subtree_" << idx << " nodes: " << countNodes(sub) << "\n";
//        ++idx;
//    }*/
//
//    int total = countNodes(&root);
//    /*for (const auto& sub : subTrees) total += countNodes(sub); */
//
//    std::cout << "\n[INFO] Total nodes in all trees: " << total << "\n";
//    std::cout << "[INFO] Approx memory usage: " << total * sizeof(Node) / 1024.0 << " KB\n";
//    return 0;
//}





// ==========================================================================
//                    Partitionable path tree extended
// ==========================================================================

//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <string>
//#include <sstream>
//#include <unordered_map>
//#include "nlohmann/json.hpp"
//#include <array>
//#include <algorithm>
//#include <iomanip>
//#include <memory>
//
//using json = nlohmann::json;
//
//struct Node {
//    std::string name;
//    std::vector<Node> children;
//    bool reserved = false;
//    std::array<uint8_t, 10> flags = { 0 };
//
//    Node(const std::string& n = "") : name(n) {}
//};
//
////struct Node1 {
////    std::string name;
////    std::vector<Node1> children;
////    bool reserved = false;
////    std::array<uint8_t, 10> flags = { 0 };
////	std::vector<unsigned long> currentFileOffsets; // For storing file offsets in the current subtree
////	std::vector<unsigned long> previousFileOffsets; // For storing file offsets in the previous subtree
////
////    Node1(const std::string& n = "") : name(n) {}
////};
//
//using Path = std::string;
//using IndexMap = std::unordered_map<Path, std::vector<std::string>>;
//
//// Serialize a node as JSON
//json serializeNode(const Node& node) {
//    json j;
//    j["name"] = node.name;
//    j["reserved"] = node.reserved;
//    j["flags"] = node.flags;
//    j["children"] = json::array();
//    for (const auto& child : node.children) {
//        j["children"].push_back(serializeNode(child));
//    }
//    return j;
//}
//
//// Write a subtree to file
//void writeSubtreeToFile(const Node& subtree, const std::string& filename) {
//    std::ofstream out(filename);
//    out << serializeNode(subtree).dump(2);
//}
//
//// Split tree into subtrees and record index
//int splitTree(Node& node, int maxNodes, IndexMap& index, const Path& path, int& subtreeCount) {
//    int total = 1;
//    std::vector<Node> newChildren;
//
//    for (auto& child : node.children) {
//        int subtreeSize = splitTree(child, maxNodes, index, path + "/" + child.name, subtreeCount);
//
//        if (subtreeSize >= maxNodes) {
//            std::string filename = "subtree_" + std::to_string(++subtreeCount) + ".json";
//            writeSubtreeToFile(child, filename);
//            index[path].push_back(filename);
//        }
//        else {
//            newChildren.push_back(std::move(child));
//        }
//        total += subtreeSize;
//    }
//
//    node.children = std::move(newChildren);
//    return total;
//}
//
//// Search for a path inside a subtree JSON
//bool searchPathInSubtree(const json& node, std::vector<std::string>::iterator it, std::vector<std::string>::iterator end) {
//    if (it == end) return true;
//    for (const auto& child : node["children"]) {
//        if (child["name"] == *it) {
//            return searchPathInSubtree(child, it + 1, end);
//        }
//    }
//    return false;
//}
//
//bool searchPath(const std::string& fullPath, const IndexMap& index) {
//    for (const auto& [parentPath, subtreeFiles] : index) {
//        if (fullPath.find(parentPath) == 0) {
//            for (const auto& file : subtreeFiles) {
//                std::ifstream in(file);
//                if (!in) continue;
//                json subtree = json::parse(in);
//
//                std::vector<std::string> parts;
//                std::istringstream ss(fullPath.substr(parentPath.size()));
//                std::string token;
//                while (std::getline(ss, token, '/')) {
//                    if (!token.empty()) parts.push_back(token);
//                }
//
//                if (searchPathInSubtree(subtree, parts.begin(), parts.end())) {
//                    return true;
//                }
//            }
//        }
//    }
//    return false;
//}
//
//// Sample data generator
//void generateSample(Node& root, int depth, int fanout, const std::string& prefix) {
//    if (depth == 0) return;
//    for (int i = 0; i < fanout; ++i) {
//        Node child(prefix + "_" + std::to_string(i));
//        generateSample(child, depth - 1, fanout, prefix + "_" + std::to_string(i));
//        root.children.push_back(std::move(child));
//    }
//}
//
//int main() {
//
//	/*std::cout << "size of new node is " << sizeof(Node1) << " bytes\n";
//    return 0;*/
//
//    Node root("root");
//    generateSample(root, 3, 10, "folder"); // Generates 1111 nodes
//
//    IndexMap index;
//    int subtreeCount = 0;
//    splitTree(root, 100, index, root.name, subtreeCount);
//
//    // Write index file
//    json jIndex(index);
//    std::ofstream("subtree_index.json") << jIndex.dump(2);
//
//    // Test search
//    std::string queryPath = "root/folder_0/folder_0_0";
//    if (searchPath(queryPath, index)) {
//        std::cout << "Found: " << queryPath << "\n";
//    }
//    else {
//        std::cout << "Not found: " << queryPath << "\n";
//    }
//
//    return 0;
//}



// ===============================================================================================================
//              Partitionable path tree extended with file offsets and raw pointers instead of vector
// ===============================================================================================================


// Node1Test.cpp
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <chrono>

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
//    void allocateOffsets(uint8_t count1, uint8_t count2) {
//        size1 = count1;
//        size2 = count2;
//        if (count1 > 0) {
//            temp1 = malloc(count1 * sizeof(uint64_t)); // example offset size
//            std::memset(temp1, 0, count1 * sizeof(uint64_t));
//        }
//        if (count2 > 0) {
//            temp2 = malloc(count2 * sizeof(uint64_t));
//            std::memset(temp2, 0, count2 * sizeof(uint64_t));
//        }
//    }
//
//    void allocateChildren(uint32_t count) {
//        size3 = count;
//        if (count > 0) {
//            temp3 = malloc(count * sizeof(Node1*)); // Array of Node1* pointers
//            std::memset(temp3, 0, count * sizeof(Node1*));
//        }
//    }
//
//    void setName(const std::string& str) {
//        if (str.empty()) return;
//        name = (char*)malloc(str.size() + 1);
//        std::strcpy(name, str.c_str());
//    }
//
//    // simulate resize children
//    void resizeChildren(uint32_t newCount) {
//        void* newBlock = malloc(newCount * sizeof(Node1*));
//        std::memset(newBlock, 0, newCount * sizeof(Node1*));
//        if (temp3 && size3 > 0) {
//            std::memcpy(newBlock, temp3, std::min(size3, newCount) * sizeof(Node1*));
//            free(temp3);
//        }
//        temp3 = newBlock;
//        size3 = newCount;
//    }
//};
//
//void testCase(const std::string& label, bool withName, bool withOffsets) {
//    const int COUNT = 10'000'000;
//    std::vector<Node1*> nodes;
//    nodes.reserve(COUNT);
//
//    auto start = std::chrono::high_resolution_clock::now();
//
//    for (int i = 0; i < COUNT; ++i) {
//        Node1* node = new Node1();
//        if (withName) node->setName("abcdefghij"); // 10 chars
//        if (withOffsets) node->allocateOffsets(2, 2);
//        node->allocateChildren(10);
//        nodes.push_back(node);
//    }
//
//    auto end = std::chrono::high_resolution_clock::now();
//    std::chrono::duration<double> duration = end - start;
//    std::cout << label << " → Created " << COUNT << " nodes in "
//        << duration.count() << " seconds.\n";
//
//    for (Node1* node : nodes) delete node;
//}


#include "../Node1.h"


int main() {
   // testCase("Tree: 10 children per node, depth 7", 10, 7);
    testCase("Test 1: Empty names and offsets", 10, 7, false, false);
    testCase("Test 2: Name (10 chars), no offsets", 10, 7, true, false);
    testCase("Test 3: Name (10 chars) + 2x offsets", 10, 7, true, true);
    return 0;
}

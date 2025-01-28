#include "huffman_tree.h"
#include <iostream>  // for debugging if needed

void Huffman::computeFrequencies(const std::vector<unsigned char> &data) {

    for (unsigned char c : data) {
         freqMap[c]++;
    }

}
// Build the Huffman tree and generate codes
void Huffman::build(const std::vector<unsigned char> &data) {
    if (data.empty()) {
        root = nullptr;
        return;
    }
    freqMap.clear();
    codes.clear();
    root = nullptr;

    computeFrequencies(data);
    buildHuffmanTree();
    generateCodes(root);
}
void Huffman::buildHuffmanTree() {
    // Create a min-heap (priority queue) of HuffmanNodes
    // Push all (char, frequency) pairs into minHeap
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareNode> minHeap;
    for (auto pair : freqMap) {
        HuffmanNode *node = new HuffmanNode(pair.first,pair.second);
        minHeap.push(node);
    }
    while (minHeap.size() > 1) {
        HuffmanNode *smallest = minHeap.top();
        minHeap.pop();
        HuffmanNode *secondSmallest = minHeap.top();
        minHeap.pop();
        HuffmanNode *parent = new HuffmanNode('\0',smallest->freq + secondSmallest->freq);
        parent->left = smallest;
        parent->right = secondSmallest;
        minHeap.push(parent);
    }


    root = minHeap.top();
    minHeap.pop();

}

void  Huffman::generateCodesHelper(HuffmanNode* node,
const std::string &path,std::unordered_map<unsigned char, std::string> &codes) {
    if (!node) return;
    if(node->left == nullptr && node->right == nullptr) {
        codes[node->c] = path;
    }
    generateCodesHelper(node->left, path + '0', codes);
    generateCodesHelper(node->right, path + '1', codes);

}

void Huffman::generateCodes(HuffmanNode* root) {


    generateCodesHelper(root, "", codes);
}

std::string Huffman::encode(const std::vector<unsigned char> &data) {
    if (data.empty()) {
        return "";
    }
    std::string encoded;
    for (unsigned char byte : data) {
        encoded += codes[byte];
    }
    
    
    return encoded;
}

std::vector<unsigned char> Huffman::decode(const std::string &encoded) {
    if (encoded.empty()) {
        return {};
    }
    std::vector<unsigned char> decoded;
    auto copy = root;
    for (char c : encoded) {
        if (c == '0') {
            copy = copy->left;
        }
        if (c == '1') {
            copy = copy->right;
        }
        if (copy->left == nullptr && copy->right == nullptr) {
            decoded.push_back(copy->c);
            copy = root;
        }
    }
    return decoded;
}
// Destructor to clean up memory
Huffman::~Huffman() {
    deleteTree(root);
}

void Huffman::deleteTree(HuffmanNode *node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

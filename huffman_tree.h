#ifndef HUFFMAN_TREE_H
#define HUFFMAN_H
#include "huffman_node.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>



struct CompareNode {
    bool operator()(HuffmanNode* left, HuffmanNode* right) {
        // Return true if left's frequency is larger, so smaller freq has higher priority
        return (left->freq > right->freq);
    }
};

class Huffman {
private:
    HuffmanNode *root;
    std::unordered_map<unsigned char, unsigned> freqMap;
    std::unordered_map<unsigned char, std::string> codes;

    void computeFrequencies(const std::vector<unsigned char> &data);
    void buildHuffmanTree();
    void generateCodes(HuffmanNode *node);
    static void  generateCodesHelper(HuffmanNode* node, const std::string &path,
                                                std::unordered_map<unsigned char, std::string> &codes);
    void deleteTree(HuffmanNode *node);

public:
    Huffman() : root(nullptr) {}
    ~Huffman();

    void build(const std::vector<unsigned char> &data);
    std::string encode(const std::vector<unsigned char> &data);
    std::vector<unsigned char> decode(const std::string &encoded);
    const std::unordered_map<unsigned char, std::string> &getCodes() const { return codes; }
};
#endif 

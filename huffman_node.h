struct HuffmanNode{
    char c;
    unsigned int freq;
    HuffmanNode *left;
    HuffmanNode *right;
    HuffmanNode(char c, unsigned int freq)
        : c(c), freq(freq), left(nullptr), right(nullptr) {}
};
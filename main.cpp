#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include "huffman_tree.h"
std::vector<unsigned char> packBits(const std::string &bitString, int &totalBits) {
    std::vector<unsigned char> packedData;
    unsigned char currentByte = 0;
    int bitCount = 0;

    totalBits = bitString.size(); // Store the total number of bits

    for (char bit : bitString) {
        currentByte = (currentByte << 1) | (bit - '0'); // Add the bit to the current byte
        bitCount++;

        if (bitCount == 8) {
            packedData.push_back(currentByte); // Store the byte when it's full
            currentByte = 0;
            bitCount = 0;
        }
    }

    if (bitCount > 0) { // Handle leftover bits
        currentByte <<= (8 - bitCount); // Pad remaining bits with zeros
        packedData.push_back(currentByte);
    }

    return packedData;
}

std::string unpackBits(const std::vector<unsigned char> &packedData, int totalBits) {
    std::string bitString;
    int bitsProcessed = 0;

    for (size_t i = 0; i < packedData.size(); ++i) {
        unsigned char byte = packedData[i];
        for (int j = 7; j >= 0; --j) { // Extract bits from most significant to least significant
            if (bitsProcessed == totalBits) {
                return bitString; // Stop if all valid bits are unpacked
            }
            bitString += ((byte >> j) & 1) ? '1' : '0';
            bitsProcessed++;
        }
    }

    return bitString;
}

// Read a file as binary data
std::vector<unsigned char> readBinaryFile(const std::string &filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file for reading.");
    }
    return std::vector<unsigned char>((std::istreambuf_iterator<char>(file)),
                                       std::istreambuf_iterator<char>());
}

// Write binary data to a file
void writeBinaryFile(const std::string &filePath, const std::vector<unsigned char> &data) {
    std::ofstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing.");
    }
    file.write(reinterpret_cast<const char *>(data.data()), data.size());
}

// Save metadata and packed data to a compressed file
void saveCompressedFile(const std::string &filePath, const std::vector<unsigned char> &packedData,
                        const std::unordered_map<unsigned char, std::string> &codeMap, int totalBits) {
    std::ofstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to write compressed file.");
    }

    // Save total bits
    file.write(reinterpret_cast<const char *>(&totalBits), sizeof(totalBits));

    // Save the code map size
    int mapSize = codeMap.size();
    file.write(reinterpret_cast<const char *>(&mapSize), sizeof(mapSize));

    // Save the code map
    for (const auto &entry : codeMap) {
        unsigned char byte = entry.first;
        std::string code = entry.second;
        int codeLength = code.size();
        file.write(reinterpret_cast<const char *>(&byte), sizeof(byte));
        file.write(reinterpret_cast<const char *>(&codeLength), sizeof(codeLength));
        file.write(code.data(), code.size());
    }

    // Save packed data
    file.write(reinterpret_cast<const char *>(packedData.data()), packedData.size());
}

// Load metadata and packed data from a compressed file
void loadCompressedFile(const std::string &filePath, std::vector<unsigned char> &packedData,
                        std::unordered_map<unsigned char, std::string> &codeMap, int &totalBits) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to read compressed file.");
    }

    // Load total bits
    file.read(reinterpret_cast<char *>(&totalBits), sizeof(totalBits));

    // Load the code map size
    int mapSize;
    file.read(reinterpret_cast<char *>(&mapSize), sizeof(mapSize));

    // Load the code map
    for (int i = 0; i < mapSize; ++i) {
        unsigned char byte;
        int codeLength;
        file.read(reinterpret_cast<char *>(&byte), sizeof(byte));
        file.read(reinterpret_cast<char *>(&codeLength), sizeof(codeLength));
        std::string code(codeLength, '\0');
        file.read(&code[0], codeLength);
        codeMap[byte] = code;
    }

    // Load packed data
    packedData = std::vector<unsigned char>((std::istreambuf_iterator<char>(file)),
                                            std::istreambuf_iterator<char>());
}

int main() {
    try {
        // File paths
        std::string inputFilePath = "input.bin";
        std::string compressedFilePath = "compressed.huff";
        std::string outputFilePath = "output.bin";

        
        std::vector<unsigned char> fileData = readBinaryFile(inputFilePath);

        
        Huffman huffman;
        huffman.build(fileData);

        
        std::string encodedData = huffman.encode(fileData);

        
        int totalBits;
        std::vector<unsigned char> packedData = packBits(encodedData, totalBits);

        
        saveCompressedFile(compressedFilePath, packedData, huffman.getCodes(), totalBits);
        std::cout << "Compression completed and saved to " << compressedFilePath << std::endl;

        
        std::vector<unsigned char> loadedPackedData;
        std::unordered_map<unsigned char, std::string> loadedCodeMap;
        int loadedTotalBits;
        loadCompressedFile(compressedFilePath, loadedPackedData, loadedCodeMap, loadedTotalBits);

       
        std::string unpackedData = unpackBits(loadedPackedData, loadedTotalBits);

        
        std::vector<unsigned char> decodedData = huffman.decode(unpackedData);

        
        writeBinaryFile(outputFilePath, decodedData);
        std::cout << "Decompression completed and saved to " << outputFilePath << std::endl;

        
        if (fileData == decodedData) {
            std::cout << "Verification successful: Original and decompressed files match!" << std::endl;
        } else {
            std::cerr << "Verification failed: Original and decompressed files do not match." << std::endl;
        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

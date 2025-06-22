#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <array>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

struct Node {
    unsigned char symbol;
    uint64_t freq;
    int left;   // index of left child in nodes vector, -1 if none
    int right;  // index of right child, -1 if none
};

// builds frequency map from file
std::array<uint64_t,256> buildFrequencyMap(const std::string& inFile);
// builds Huffman tree nodes and returns root index
int buildHuffmanTree(const std::array<uint64_t,256>& freq,
                     std::vector<Node>& nodes);
// fills codeMap: symbol -> bitstring
void buildCodeMap(int idx, const std::vector<Node>& nodes,
                  const std::string& prefix,
                  std::unordered_map<unsigned char,std::string>& codeMap);
// serialize tree in preorder
void writeTree(int idx, const std::vector<Node>& nodes, std::ostream& out);
// read tree, return root index
int readTree(std::istream& in, std::vector<Node>& nodes);
// encode and decode streams
void encodeData(std::istream& in, class BitWriter& writer,
                const std::unordered_map<unsigned char,std::string>& codeMap);
void decodeData(class BitReader& reader, std::ostream& out,
                int rootIdx, const std::vector<Node>& nodes);

#endif
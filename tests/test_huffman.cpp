#include "Huffman.hpp"
#include "BitWriter.hpp"
#include "BitReader.hpp"
#include <sstream>
#include <cassert>

int main() {
    std::string data = "this is a test for huffman encoding";
    // build frequency directly from data
    std::array<uint64_t,256> freq{};
    for (unsigned char c : data) freq[c]++;

    std::vector<Node> nodes;
    int root = buildHuffmanTree(freq, nodes);
    std::unordered_map<unsigned char,std::string> codeMap;
    buildCodeMap(root, nodes, "", codeMap);
    
    // encode data from memory
    std::istringstream rawIn(data);
    std::ostringstream compressed;
    writeTree(root, nodes, compressed);
    BitWriter writer(compressed);
    encodeData(rawIn, writer, codeMap);
    writer.flush();

    // decode back
    std::istringstream inStream(compressed.str());
    std::vector<Node> nodes2;
    int root2 = readTree(inStream, nodes2);
    BitReader reader(inStream);
    std::ostringstream out;
    decodeData(reader, out, root2, nodes2);

    assert(out.str() == data);
    return 0;
}
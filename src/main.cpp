#include "BitReader.hpp"
#include "BitWriter.hpp"
#include "Huffman.hpp"
#include <fstream>
#include <iostream>


int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: zipper -c|-d <input> <output>\n";
        return 1;
    }
    std::string mode = argv[1];
    std::string inFile = argv[2];
    std::string outFile = argv[3];

    if (mode == "-c") {
        auto freq = buildFrequencyMap(inFile);
        std::vector<Node> nodes;
        int root = buildHuffmanTree(freq, nodes);
        std::unordered_map<unsigned char, std::string> codeMap;
        buildCodeMap(root, nodes, "", codeMap);

        std::ifstream in(inFile, std::ios::binary);
        std::ofstream out(outFile, std::ios::binary);
        writeTree(root, nodes, out);
        BitWriter writer(out);
        encodeData(in, writer, codeMap);
        writer.flush();

    } else if (mode == "-d") {
        std::ifstream in(inFile, std::ios::binary);
        std::ofstream out(outFile, std::ios::binary);
        std::vector<Node> nodes;
        int root = readTree(in, nodes);
        BitReader reader(in);
        decodeData(reader, out, root, nodes);

    } else {
        std::cerr << "Unknown mode " << mode << "\n";
        return 1;
    }
    return 0;
}
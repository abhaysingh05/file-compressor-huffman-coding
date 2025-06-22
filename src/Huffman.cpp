#include "Huffman.hpp"
#include "BitReader.hpp"
#include "BitWriter.hpp"
#include <fstream>
#include <queue>


using ArrayFreq = std::array<uint64_t, 256>;

ArrayFreq buildFrequencyMap(const std::string &inFile) {
    ArrayFreq freq{};
    std::ifstream in(inFile, std::ios::binary);
    unsigned char ch;
    while (in.read(reinterpret_cast<char *>(&ch), 1)) freq[ch]++;
    return freq;
}

struct PQItem {
    int idx;
    uint64_t freq;
    bool operator>(const PQItem &o) const { return freq > o.freq; }
};

int buildHuffmanTree(const ArrayFreq &freq, std::vector<Node> &nodes) {
    std::priority_queue<PQItem, std::vector<PQItem>, std::greater<>> pq;
    nodes.clear();
    for (int i = 0; i < 256; ++i)
        if (freq[i]) {
            nodes.push_back(Node{(unsigned char)i, freq[i], -1, -1});
            pq.push({(int)nodes.size() - 1, freq[i]});
        }
    if (pq.empty()) return -1;
    while (pq.size() > 1) {
        auto a = pq.top();
        pq.pop();
        auto b = pq.top();
        pq.pop();
        nodes.push_back(Node{0, a.freq + b.freq, a.idx, b.idx});
        pq.push({(int)nodes.size() - 1, a.freq + b.freq});
    }
    return pq.top().idx;
}

void buildCodeMap(int idx, const std::vector<Node> &nodes, const std::string &pre,
                  std::unordered_map<unsigned char, std::string> &codeMap) {
    const Node &n = nodes[idx];
    if (n.left < 0 && n.right < 0)
        codeMap[n.symbol] = pre;
    else {
        if (n.left >= 0) buildCodeMap(n.left, nodes, pre + '0', codeMap);
        if (n.right >= 0) buildCodeMap(n.right, nodes, pre + '1', codeMap);
    }
}

void writeTree(int idx, const std::vector<Node> &nodes, std::ostream &out) {
    const Node &n = nodes[idx];
    if (n.left < 0 && n.right < 0) {
        out.put('1');
        out.put(n.symbol);
    } else {
        out.put('0');
        writeTree(n.left, nodes, out);
        writeTree(n.right, nodes, out);
    }
}

int readTree(std::istream &in, std::vector<Node> &nodes) {
    char f;
    if (!in.get(f)) return -1;
    if (f == '1') {
        unsigned char s;
        in.read(reinterpret_cast<char *>(&s), 1);
        nodes.push_back({s, 0, -1, -1});
        return (int)nodes.size() - 1;
    }
    int L = readTree(in, nodes), R = readTree(in, nodes);
    nodes.push_back({0, 0, L, R});
    return (int)nodes.size() - 1;
}

void encodeData(std::istream &in, BitWriter &w, const std::unordered_map<unsigned char, std::string> &m) {
    unsigned char c;
    while (in.read(reinterpret_cast<char *>(&c), 1)) w.writeBits(m.at(c));
}

void decodeData(BitReader &r, std::ostream &out, int root, const std::vector<Node> &nodes) {
    int cur = root;
    bool b;
    while (r.readBit(b)) {
        cur = b ? nodes[cur].right : nodes[cur].left;
        const Node &n = nodes[cur];
        if (n.left < 0 && n.right < 0) {
            out.put(n.symbol);
            cur = root;
        }
    }
}
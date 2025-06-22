#include "BitWriter.hpp"

BitWriter::BitWriter(std::ostream &outStream)
    : out(outStream), buffer(0), count(0) {}

void BitWriter::writeBit(bool bit) {
    buffer = (buffer << 1) | static_cast<unsigned char>(bit);
    if (++count == 8) {
        out.put(buffer);
        buffer = 0;
        count = 0;
    }
}

void BitWriter::writeBits(const std::string &bits) {
    for (char c : bits) {
        writeBit(c == '1');
    }
}

void BitWriter::flush() {
    if (count > 0) {
        buffer <<= (8 - count);
        out.put(buffer);
        buffer = 0;
        count = 0;
    }
}
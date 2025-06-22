#include "BitReader.hpp"

BitReader::BitReader(std::istream &inStream): in(inStream), buffer(0), count(0) {}

bool BitReader::readBit(bool &bit) {
    if (count == 0) {
        char ch;
        if (!in.get(ch)) return false;
        buffer = static_cast<unsigned char>(ch);
        count = 8;
    }
    bit = ((buffer >> 7) & 1);
    buffer <<= 1;
    --count;
    return true;
}
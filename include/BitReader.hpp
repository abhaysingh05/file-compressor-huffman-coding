#ifndef BIT_READER_HPP
#define BIT_READER_HPP

#include <istream>

class BitReader {
public:
    explicit BitReader(std::istream& in);
    // returns false when stream ended
    bool readBit(bool& bit);

private:
    std::istream& in;
    unsigned char buffer;
    int count;
};

#endif // BIT_READER_HPP
#ifndef BIT_WRITER_HPP
#define BIT_WRITER_HPP

#include <ostream>
#include <string>

class BitWriter {
public:
    BitWriter(std::ostream& out);
    void writeBit(bool bit);
    void writeBits(const std::string& bits);
    void flush();

private:
    std::ostream& out;
    unsigned char buffer;
    int count;
};

#endif
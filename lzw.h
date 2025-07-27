#ifndef LZW_H
#define LZW_H

#include <string>
#include <stdexcept> // Required for std::runtime_error

namespace LZW {

    constexpr int CODE_BIT_WIDTH = 12;
    constexpr int MAX_DICT_SIZE = 1 << CODE_BIT_WIDTH;

    void compress(const std::string& input_filename, const std::string& output_filename);

    void decompress(const std::string& input_filename, const std::string& output_filename);

}

#endif // LZW_H

#include "lzw.h"
#include "bitstream.h"
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>

namespace LZW {

void compress(const std::string& input_filename, const std::string& output_filename) {
    std::ifstream input(input_filename, std::ios::binary);
    if (!input) {
        throw std::runtime_error("Cannot open input file '" + input_filename + "'");
    }
    
    std::ofstream output(output_filename, std::ios::binary);
    if (!output) {
        throw std::runtime_error("Cannot create output file '" + output_filename + "'");
    }

    std::unordered_map<std::string, int> dictionary;
    for (int i = 0; i < 256; ++i) {
        dictionary[std::string(1, static_cast<char>(i))] = i;
    }

    int next_code = 256;
    int current_bit_width = 9;
    int max_codes_for_width = 1 << current_bit_width;

    BitWriter writer(output);
    std::string current_string;
    char c;

    // Prime the pump by reading the first character
    if (input.get(c)) {
        current_string = std::string(1, c);
    }

    // Read the rest of the file
    while (input.get(c)) {
        std::string string_with_char = current_string + c;
        if (dictionary.count(string_with_char)) {
            // Found a longer match, continue building the string
            current_string = string_with_char;
        } else {
            // No match found, write the code for the last known string
            writer.write(dictionary.at(current_string), current_bit_width);

            // Add the new string to the dictionary
            if (next_code < MAX_DICT_SIZE) {
                dictionary[string_with_char] = next_code++;
            }

            // Check if we need to increase the bit width
            if (next_code == max_codes_for_width && current_bit_width < CODE_BIT_WIDTH) {
                current_bit_width++;
                max_codes_for_width = 1 << current_bit_width;
            }
            
            // Start a new string with the current character
            current_string = std::string(1, c);
        }
    }

    // Write the code for the final string in the buffer
    if (!current_string.empty()) {
        writer.write(dictionary.at(current_string), current_bit_width);
    }
}

void decompress(const std::string& input_filename, const std::string& output_filename) {
    std::ifstream input(input_filename, std::ios::binary);
    if (!input) {
        throw std::runtime_error("Cannot open input file '" + input_filename + "'");
    }
    
    std::ofstream output(output_filename, std::ios::binary);
    if (!output) {
        throw std::runtime_error("Cannot create output file '" + output_filename + "'");
    }

    std::unordered_map<int, std::string> dictionary;
    for (int i = 0; i < 256; ++i) {
        dictionary[i] = std::string(1, static_cast<char>(i));
    }

    int next_code = 256;
    int current_bit_width = 9;
    int max_codes_for_width = 1 << current_bit_width;

    BitReader reader(input);
    int previous_code;
    
    // Bootstrap the process by reading the first code
    if (!reader.read(previous_code, current_bit_width)) {
        return; // File is empty
    }
    
    std::string previous_string = dictionary.at(previous_code);
    output.write(previous_string.c_str(), previous_string.length());

    int current_code;
    while (reader.read(current_code, current_bit_width)) {
        std::string current_entry;
        if (dictionary.count(current_code)) {
            // Code is in the dictionary
            current_entry = dictionary.at(current_code);
        } else if (current_code == next_code) {
            // Special case: KWKWK
            current_entry = previous_string + previous_string[0];
        } else {
            // If we get here, the file is corrupt
            throw std::runtime_error("Invalid code in compressed stream.");
        }

        output.write(current_entry.c_str(), current_entry.length());

        // Add the new entry to the dictionary
        if (next_code < MAX_DICT_SIZE) {
            dictionary[next_code++] = previous_string + current_entry[0];
        }

        // Check if we need to increase the bit width
        if (next_code == max_codes_for_width && current_bit_width < CODE_BIT_WIDTH) {
            current_bit_width++;
            max_codes_for_width = 1 << current_bit_width;
        }
        
        previous_string = current_entry;
    }
}

} // namespace LZW

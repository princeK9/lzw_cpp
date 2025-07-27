#include <iostream>
#include <string>
#include <chrono>
#include <stdexcept>
#include <fstream>   // Required for file comparison
#include <algorithm> // Required for std::equal
#include <iterator>  // Required for std::istreambuf_iterator

#include "lzw.h"

void print_usage(const std::string& program_name) { // called if user runs incorrect command
    std::cerr << "LZW File Compression Utility\n"
              << "----------------------------\n"
              << "Usage: " << program_name << " <mode> <input_file> <output_file>\n"
              << "  <mode>: -c to compress\n"
              << "          -d to decompress\n"
              << "          -v to verify two files\n";
}

// A function to compare two files byte-by-byte
bool compare_files(const std::string& filename1, const std::string& filename2) {
    std::ifstream f1(filename1, std::ios::binary | std::ios::ate);
    std::ifstream f2(filename2, std::ios::binary | std::ios::ate);

    if (!f1.is_open() || !f2.is_open()) {
        // One of the files couldn't be opened
        return false;
    }

    // First, check if file sizes are the same. If not, they can't be identical.
    if (f1.tellg() != f2.tellg()) {
        return false;
    }

    // Rewind files to the beginning
    f1.seekg(0, std::ios::beg);
    f2.seekg(0, std::ios::beg);

    // Use a standard C++ algorithm to efficiently compare the file contents
    return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                      std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(f2.rdbuf()));
}


int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Error: Invalid number of arguments.\n\n";
        print_usage(argv[0]);
        return 1;
    }

    const std::string mode = argv[1];
    const std::string file1 = argv[2];
    const std::string file2 = argv[3];
    
    auto start_time = std::chrono::high_resolution_clock::now();

    try {
        if (mode == "-c") {
            std::cout << "Compressing '" << file1 << "' -> '" << file2 << "'..." << std::endl;
            LZW::compress(file1, file2);
            std::cout << "Compression successful." << std::endl;

        } else if (mode == "-d") {
            std::cout << "Decompressing '" << file1 << "' -> '" << file2 << "'..." << std::endl;
            LZW::decompress(file1, file2);
            std::cout << "Decompression successful." << std::endl;

        } else if (mode == "-v") {
            std::cout << "Verifying '" << file1 << "' and '" << file2 << "'..." << std::endl;
            if (compare_files(file1, file2)) {
                std::cout << "Verification successful: Files are identical." << std::endl;
            } else {
                std::cout << "Verification FAILED: Files are different." << std::endl;
                return 1; // Return error code on failure
            }

        } else {
            std::cerr << "Error: Invalid mode '" << mode << "'.\n\n";
            print_usage(argv[0]);
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    std::cout << "Operation completed in " << duration.count() << " ms." << std::endl;

    return 0;
}

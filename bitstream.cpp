#include "bitstream.h"

// --- BitReader Implementation ---

BitReader::BitReader(std::ifstream& in)
    // This is a member initializer list. It's an efficient way to set initial values.
    : input_stream(in), buffer(0), bit_count(0) {
}

bool BitReader::read(int& code, int num_bits) {
    // Reset the output code to 0 before we start filling it with bits.
    code = 0;

    // Loop to read the requested number of bits, one at a time.
    for (int i = 0; i < num_bits; ++i) {
        // If our 8-bit buffer is empty, we need to read a new byte from the file.
        if (bit_count == 0) {
            char next_byte;
            // Attempt to get the next character (byte) from the input stream.
            if (!input_stream.get(next_byte)) {
                return false; // Failed to read, likely End-Of-File.
            }
            // Store the read byte in our buffer.
            buffer = static_cast<unsigned char>(next_byte);
            // We now have 8 new bits available to be read.
            bit_count = 8;
        }

        // Extract the most significant bit (MSB) from the buffer.
        int bit = (buffer >> 7) & 1;

        // Append this new bit to our code.
        code = (code << 1) | bit;

        // "Consume" the bit we just read by shifting the buffer to the left.
        buffer <<= 1;
        bit_count--;
    }
    return true; // Successfully read all requested bits.
}


// --- BitWriter Implementation ---

BitWriter::BitWriter(std::ofstream& out)
    : output_stream(out), buffer(0), bit_count(0) {
}

// The destructor is automatically called when the BitWriter object is destroyed.
BitWriter::~BitWriter() {
    // If there are any leftover bits in the buffer, we must write them.
    if (bit_count > 0) {
        // Left-shift the buffer to pad the remaining space with zeros.
        // For example, if we have 3 bits '101', the buffer is 00000101.
        // Shifting by (8-3)=5 results in 10100000.
        buffer <<= (8 - bit_count);
        output_stream.put(buffer);
    }
}

void BitWriter::write(int code, int num_bits) {
    // Loop through the code's bits, from most significant to least significant.
    for (int i = num_bits - 1; i >= 0; --i) {
        // Isolate the i-th bit of the code.
        int bit = (code >> i) & 1;

        // Add this bit to our buffer by shifting the buffer left and ORing the bit.
        buffer = (buffer << 1) | bit;
        bit_count++;

        // If the buffer is now full (has 8 bits), write it to the file.
        if (bit_count == 8) {
            output_stream.put(buffer);
            // Reset the buffer and counter for the next byte.
            buffer = 0;
            bit_count = 0;
        }
    }
}

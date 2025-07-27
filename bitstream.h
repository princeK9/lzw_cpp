#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <fstream> // For std::ifstream and std::ofstream

// A helper class to read data bit-by-bit from an input stream.
class BitReader {
public:
    /**
     * @brief Constructs a BitReader that reads from the given input stream.
     * @param in A reference to the input file stream to read from.
     */
    BitReader(std::ifstream& in);

    /**
     * @brief Reads a single code of 'num_bits' length from the stream.
     *
     * This function attempts to read enough bits from the input stream to form a
     * complete code. It manages an internal buffer to handle reading byte-by-byte
     * from the file and dealing out bits as requested.
     *
     * @param code A reference to an integer where the read code will be stored.
     * @param num_bits The number of bits to read for this code (e.g., 9, 10, 12).
     * @return true if a full code was successfully read, false if the end of the file was reached prematurely.
     */
    bool read(int& code, int num_bits);

private:
    std::ifstream& input_stream; // Reference to the file stream we are reading from.
    unsigned char buffer;        // An 8-bit buffer to hold a byte read from the file.
    int bit_count;               // How many bits are still available to be read from the buffer.
};

// A helper class to write data bit-by-bit to an output stream.
class BitWriter {
public:
    /**
     * @brief Constructs a BitWriter that writes to the given output stream.
     * @param out A reference to the output file stream to write to.
     */
    BitWriter(std::ofstream& out);

    /**
     * @brief Destructor that automatically flushes any remaining bits.
     *
     * When the BitWriter object goes out of scope (e.g., at the end of a function),
     * this ensures that any bits left over in the buffer are written to the file,
     * preventing data loss.
     */
    ~BitWriter();

    /**
     * @brief Writes a single code using 'num_bits' length to the stream.
     *
     * This function takes an integer code, breaks it down into its constituent bits,
     * and adds them one by one to an internal buffer. When the buffer is full (8 bits),
     * it is written to the output file as a single byte.
     *
     * @param code The integer code to write.
     * @param num_bits The number of bits to use for this code (e.g., 9, 10, 12).
     */
    void write(int code, int num_bits);

private:
    std::ofstream& output_stream; // Reference to the file stream we are writing to.
    unsigned char buffer;         // An 8-bit buffer to accumulate bits before writing.
    int bit_count;                // How many bits have been accumulated in the buffer so far.
};

#endif // BITSTREAM_H

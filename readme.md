# LZW File Compression Utility (C++)

This is a functional command-line tool for file compression and decompression built in C++. The primary purpose of this project is to serve as a portfolio piece demonstrating a strong understanding of C++ programming, classic computer science algorithms, and low-level data manipulation.

---

## What It Does

- **Lossless Compression:** Uses the Lempel-Ziv-Welch (LZW) algorithm to shrink files without losing any data. The original file can be perfectly reconstructed.
- **Command-Line Interface:** The tool is operated entirely from the terminal, accepting commands to compress, decompress, or verify files.

---

## Where It Works Best

This tool is most effective on uncompressed files with high amounts of repeating data. Good candidates include:

- `.txt` (Plain Text)
- `.log` (Log Files)
- `.csv`, `.json`, `.xml` (Structured Data)
- `.html`, `.css`, `.js` (Web & Source Code)
- `.bmp` (Uncompressed Bitmap Images)
- `.wav` (Uncompressed Audio)

---

## Technical Features

- **Variable-Width Codes:** To maximize the compression ratio, the algorithm adapts by using 9-bit codes initially and expanding up to 12-bit codes as the dictionary of patterns grows.
- **Bit-Level I/O:** The program reads and writes data bit-by-bit, not just byte-by-byte, which is essential for efficient compression. This is handled by dedicated `BitReader` and `BitWriter` classes.
- **Standard C++:** Written in modern C++17 without external dependencies, making it portable across Windows, macOS, and Linux.
- **Error Handling:** The program provides clear error messages for common issues like invalid commands or missing files.

---

## Getting Started

### Prerequisites

You must have a C++17 compatible compiler installed on your system. The most common one is `g++`, which is part of the GCC toolchain (available through MinGW on Windows).

### Compilation

Open your terminal in the project's root directory.

Run the following command:

```sh
g++ main.cpp lzw.cpp bitstream.cpp -o lzw -std=c++17
```

This will create a single executable file named `lzw` (or `lzw.exe` on Windows).

---

## How to Use the Program

The tool is run from your terminal using the following format:

```sh
./lzw <mode> <input_file> <output_file>
```

### 1. To Compress a File

Use the `-c` flag. It's recommended to use a custom extension like `.lzw` for your compressed files.

**Example:**
```sh
./lzw -c my_document.txt compressed_archive.lzw
```

### 2. To Decompress a File

Use the `-d` flag.

**Example:**
```sh
./lzw -d compressed_archive.lzw restored_document.txt
```

### 3. To Verify Files are Identical

After decompressing, you can confirm the output is a perfect match to the original using the `-v` flag.

**Example:**
```sh
./lzw -v my_document.txt restored_document.txt
```

---

## Limitations and Drawbacks

It is important to understand the limitations of this tool:

- **Educational Purpose:** This tool was built as a learning exercise and is not competitive with modern compression software like 7-Zip, WinRAR, or zstd, which use far more advanced and effective algorithms.
- **Poor Performance on Compressed Files:** This tool will perform very poorly on files that are already compressed. Attempting to compress the following file types will likely result in an output file that is larger than the original:
    - **Media:** `.jpg`, `.png`, `.gif`, `.mp3`, `.mp4`, `.mkv`
    - **Archives:** `.zip`, `.rar`, `.7z`
    - **Documents & Programs:** `.pdf`, `.exe`, `.dll`
- **Fixed Dictionary Size:** This implementation uses a fixed 12-bit code width, limiting the dictionary to 4096 entries. Once the dictionary is full, it stops learning new patterns, which can limit the compression ratio on very large, complex files.
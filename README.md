# Text-File-Compression
Using Huffman Coding

# Huffman Coding Project

This project implements Huffman Encoding and Decoding in C++ to compress and decompress text files efficiently.

## Project Structure
```
|-- compress.cpp              # Code for Huffman compression
|-- decompress.cpp            # Code for Huffman decompression
|-- huff.cpp                  # Main Huffman logic (tree building, encoding, decoding)
|-- huff.hpp                  # Huffman class and function declarations
|-- inputFile.txt             # Original input file to be compressed
|-- outputFile.txt            # Decompressed output file
|-- compressed.huf            # Compressed file (binary format)
```

## How to Run

### Requirements
- C++ compiler (GCC, Clang, MSVC)
- C++11 or later

### Compile the Code
```
# Compile compress.cpp to generate executable for compression
 g++ compress.cpp huff.cpp -o compress

# Compile decompress.cpp to generate executable for decompression
 g++ decompress.cpp huff.cpp -o decompress
```

### Compression
```
# Compress the input file
 ./compress inputFile.txt compressed.huf
```

### Decompression
```
# Decompress the compressed file
 ./decompress compressed.huf outputFile.txt
```

## How it Works

### Compression Steps
1. **Frequency Calculation:** Counts frequency of each character in the input file.
2. **Huffman Tree Building:** Constructs a binary tree based on character frequencies.
3. **Code Generation:** Assigns binary codes to each character (shorter codes for more frequent characters).
4. **File Encoding:** Writes encoded data and metadata (tree structure) to `compressed.huf`.

### Decompression Steps
1. **Metadata Extraction:** Reads Huffman tree from the compressed file.
2. **Tree Reconstruction:** Rebuilds the Huffman tree.
3. **Binary Decoding:** Decodes binary data to reconstruct the original file.
4. **Output Generation:** Writes the decompressed data to `outputFile.txt`.

## Example
```
# Sample Workflow
 g++ compress.cpp huff.cpp -o compress
 g++ decompress.cpp huff.cpp -o decompress

 ./compress inputFile.txt compressed.huf
 ./decompress compressed.huf outputFile.txt
```

- **inputFile.txt** – Original file.
- **compressed.huf** – Compressed binary file (smaller size).
- **outputFile.txt** – Decompressed file (should match the original input).

## Notes
- The Huffman coding algorithm is optimal for lossless data compression.
- Padding is handled during compression to ensure the final byte is properly formed.

Contribution:
- Feel free to fork this repository and submit pull requests for improvements or bug fixes.

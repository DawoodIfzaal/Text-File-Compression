//Header Guards to prevent header files from being included multiple times
#ifndef HUFF_HPP
#define HUFF_HPP

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;

// Node structure for Huffman tree (contains data, frequency, code, and left & right children)
struct Node {
    char data;          // Character (data) represented by this node
    unsigned freq;      // Frequency of the character in the file
    string code;        // Huffman code associated with this node
    Node* left;         // Pointer to the left child
    Node* right;        // Pointer to the right child

    // Constructor to initialize left and right children to NULL
    Node() {
        left = NULL;
        right = NULL;
    }
};

// Huffman class that will handle the compression and decompression process
class huffman {
    vector<Node*> arr;             // Array to store nodes for each character (ASCII 0-127)
    fstream inFile, outFile;       // Files for reading and writing
    string inputFileName, outputFileName; // Input and output file names
    Node* root;                    // Root node of the Huffman tree

    // Comparator used to build the priority queue (Min Heap)
    struct compare {
        bool operator()(Node* l, Node* r) {
            return l -> freq > r -> freq;  // Nodes with higher frequency should come later in the queue
        }
    };

    // Min heap (priority queue) to store nodes in increasing frequency order
    priority_queue<Node*, vector<Node*>, compare> minHeap;

    // Function to initialize the array with nodes for all characters
    void createArr();

    // Recursive function to traverse the Huffman tree and assign codes
    void traverse(Node*, string);

    // Function to convert a binary string to its decimal equivalent
    int binToDec(string);

    // Function to convert a decimal value to its binary equivalent
    string decToBin(int);

    // Function to build the Huffman tree (called by createTree())
    void buildTree(char, string&);

    // Function to create the Min Heap based on character frequencies
    void createMinHeap();

    // Function to build the Huffman tree from the Min Heap
    void createTree();

    // Function to generate the Huffman codes for each character
    void createCodes();

    // Function to save the encoded file (with Huffman codes and tree)
    void saveEncodedFile();

    // Function to save the decoded file (from the Huffman encoded file)
    void saveDecodedFile();

    // Function to retrieve and print the Huffman tree (used for debugging or visualization)
    void getTree();

public:
    // Constructor to initialize the input and output file names and create the array of nodes
    huffman(string inputFile, string outputFile) {
        this -> inputFileName = inputFile;
        this -> outputFileName = outputFile;
        createArr();  // Initialize the array of nodes
    }

    // Public function to compress the input file using Huffman coding
    void compress();

    // Public function to decompress the encoded file using the Huffman tree
    void decompress();
};

#endif

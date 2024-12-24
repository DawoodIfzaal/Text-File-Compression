#include "huff.hpp"

// Initializes the array of Node objects for all ASCII values (0-127)
void huffman::createArr(){
    for(int i = 0; i < 128; i++){
        arr.push_back(new Node());  // Allocate memory for each node
        arr[i] -> data = i;         // Set the ASCII value as the node data
        arr[i] -> freq = 0;         // Initialize frequency to 0
    }
}

// Creates the Min Heap based on character frequency in the input file
void huffman::createMinHeap(){
    char id;
    inFile.open(inputFileName, ios::in); // Open the input file for reading
    inFile.get(id);  // Read the first character

    // Increment the frequency of each character in the input file
    while(!inFile.eof()){
        arr[id] -> freq++;  // Increase the frequency of the current character
        inFile.get(id);     // Read the next character
    }

    inFile.close(); // Close the input file

    // Push nodes with non-zero frequencies into the priority queue (Min Heap)
    for(int i = 0; i < 128; i++){
        if(arr[i] -> freq > 0){
            minHeap.push(arr[i]);  // Add the node to the Min Heap
        }
    }
}

// Constructs the Huffman tree from the Min Heap
void huffman::createTree(){
    Node* left;
    Node* right;
    priority_queue<Node*, vector<Node*>, compare> temp(minHeap); // Create a temporary Min Heap

    // Merge the two nodes with the lowest frequencies until one node remains
    while(temp.size() != 1){
        left = temp.top();   // Get the node with the lowest frequency
        temp.pop();

        right = temp.top();  // Get the next node with the second lowest frequency
        temp.pop();

        root = new Node();   // Create a new root node for the merged nodes
        root -> freq = left -> freq + right -> freq;  // Set the frequency as the sum of both nodes

        root -> left = left;  // Set the left child of the root
        root -> right = right; // Set the right child of the root

        temp.push(root);  // Push the new root back into the Min Heap
    }
}

// Recursively traverses the Huffman tree to generate the Huffman codes
void huffman::traverse(Node* root, string res){
    if(root -> left == NULL && root -> right == NULL){
        root -> code = res;  // Assign the generated code to the leaf node
        return;
    }

    traverse(root -> left, res + '0');  // Traverse the left subtree with a '0'
    traverse(root -> right, res + '1'); // Traverse the right subtree with a '1'
}

// Generates Huffman codes for all characters in the tree
void huffman::createCodes(){
    traverse(root, ""); // Start the traversal from the root with an empty string
}

// Converts a binary string to its decimal equivalent
int huffman::binToDec(string s){
    int res = 0;
    for(int i = 0; i < 8; i++){
        char c = s[i];  // Get the character in the binary string
        res = (res << 1) + (c - '0');  // Shift the result and add the current bit
    }
    return res;  // Return the decimal value
}

// Saves the encoded file with the Huffman tree and compressed data
void huffman::saveEncodedFile(){
    inFile.open(inputFileName, ios::in);  // Open the input file for reading
    outFile.open(outputFileName, ios::out | ios::binary); // Open the output file for writing in binary mode
    string in = "", s = "";
    char id;    

    // Save the metadata (Huffman tree)
    in += (char)minHeap.size();  // Save the size of the Min Heap (number of unique characters)

    priority_queue<Node*, vector<Node*>, compare> temp(minHeap);  // Create a temporary Min Heap

    // Save the tree structure for decoding later
    while(!temp.empty()){
        Node* top = temp.top();  // Get the node at the top (smallest frequency)
        temp.pop();

        in += top -> data;  // Save the character data

        // Generate the Huffman code for the character
        s.assign(127 - top -> code.size(), '0');  // Pad the code to 127 bits
        s += '1';  // Add a separator '1'
        s += top -> code;  // Add the actual Huffman code

        // Save the code in chunks of 8 bits
        in += (char)binToDec(s.substr(0, 8));  // Convert the first 8 bits to a byte and save it
        for(int i = 0; i < 15; i++){
            s = s.substr(8);  // Remove the first 8 bits
            in += (char)binToDec(s.substr(0, 8));  // Save the next 8 bits
        }
    }

    s.clear();  // Clear the string for storing encoded bits

    // Encode the characters from the input file based on their Huffman codes
    inFile.get(id);  // Read the first character
    while(!inFile.eof()){
        s += arr[id] -> code;  // Append the Huffman code for the character

        // Save the encoded data in chunks of 8 bits
        while(s.length() > 8){
            in += (char)binToDec(s.substr(0, 8));  // Convert the first 8 bits to a byte and save it
            s = s.substr(8);  // Remove the first 8 bits
        }
        inFile.get(id);  // Read the next character
    }

    // Handle any remaining bits less than 8
    int count = 8 - s.length();
    if(s.length() < 8){
        s.append(count, '0');  // Append 0's to make the length 8
    }

    in += (char)binToDec(s);  // Save the remaining bits as a byte
    in += (char)count;  // Save the count of the appended 0's for decoding

    // Write the encoded data to the output file
    outFile.write(in.c_str(), in.size());  // Write the string to the file
    inFile.close();  // Close the input file
    outFile.close(); // Close the output file
}

// Compresses the input file by creating the Huffman tree, generating codes, and saving the encoded file
void huffman::compress(){
    createMinHeap();  // Create the Min Heap based on the frequency of characters
    createTree();     // Build the Huffman tree
    createCodes();    // Generate Huffman codes for all characters
    saveEncodedFile();  // Save the encoded file with metadata and compressed data
}

// Convert decimal to binary (8-bit representation)
string huffman::decToBin(int inNum){
    string result = "";

    // Loop through 8 bits and construct the binary representation
    for(int i = 7; i >= 0; i--){
        result += ((1 << i) & inNum)? '1' : '0';
    }

    return result;
}

// Build Huffman tree from character and its binary path
void huffman::buildTree(char aCode, string& path){
    Node* curr = root;

    // Traverse the path and build nodes if necessary
    for(int i = 0; i < path.length(); i++){
        if(path[i] == '0'){  // Go left if the path bit is 0
            if(curr -> left == NULL){
                curr -> left = new Node();
            }
            curr = curr -> left;
        }
        else if(path[i] == '1'){  // Go right if the path bit is 1
            if(curr -> right == NULL){
                curr -> right = new Node();
            }
            curr = curr -> right;
        }
    }
    curr -> data = aCode;  // Assign the character to the leaf node
}

// Read metadata and rebuild the Huffman tree from the input file
void huffman::getTree(){
    inFile.open(inputFileName, ios::in | ios::binary);
    
    // Read the number of unique characters (size of the Huffman tree)
    unsigned char size;
    inFile.read(reinterpret_cast<char*>(&size), 1);

    root = new Node();  // Initialize the root node of the Huffman tree

    // Read each character and its associated 16-byte binary code
    for(int i = 0; i < size; i++){
        char aCode;
        unsigned char hCode[16];
        
        inFile.read(&aCode, 1);  // Read the character
        inFile.read(reinterpret_cast<char*>(hCode), 16);  // Read the 16-byte binary path
        
        string hCodeStr = "";
        
        // Convert each byte into 8-bit binary and concatenate to form the complete path
        for(int i = 0; i < 16; i++){
            hCodeStr += decToBin(hCode[i]);
        }

        // Skip leading zeros in the path
        int j = 0;
        while(hCodeStr[j] == '0'){
            j++;
        }

        hCodeStr = hCodeStr.substr(j + 1);  // Trim the leading zeros and 1 delimiter
        buildTree(aCode, hCodeStr);  // Build the Huffman tree from the character and path
    }
    inFile.close();
}

// Decode the compressed data and write the output to a file
void huffman::saveDecodedFile(){
    inFile.open(inputFileName, ios:: in | ios:: binary);
    outFile.open(outputFileName, ios:: out);

    // Read the number of unique characters
    unsigned char size;
    inFile.read(reinterpret_cast<char*>(&size), 1);

    // Go to the last byte to read the number of padding bits
    inFile.seekg(-1, ios:: end);
    char count0;
    inFile.read(&count0, 1);

    // Skip metadata and position the file pointer at the start of compressed data
    inFile.seekg(1 + size * 17, ios::beg);

    // Read the compressed data byte by byte
    vector<unsigned char> text;
    unsigned char textseg;
    inFile.read(reinterpret_cast<char*>(&textseg), 1);
    
    // Store each byte in the vector until the end of the file
    while(!inFile.eof()){
        text.push_back(textseg);
        inFile.read(reinterpret_cast<char*>(&textseg), 1);
    }
    
    Node* curr = root;  // Start from the root of the Huffman tree
    string path;
    
    // Decode each byte in the text vector
    for(int i = 0; i < text.size() - 1; i++){
        path = decToBin(text[i]);  // Convert byte to 8-bit binary string

        // Trim the padding bits from the last byte
        if(i == text.size() - 2){
            path = path.substr(0, 8 - count0);
        }

        // Traverse the binary path through the Huffman tree
        for(int j = 0; j < path.size(); j++){
            if(path[j] == '0'){
                curr = curr -> left;
            }
            else{
                curr = curr -> right;
            }

            // If a leaf node is reached, write the character to the output file
            if(curr -> left == NULL && curr -> right == NULL){
                outFile.put(curr -> data);
                curr = root;  // Reset to the root for the next path
            }
        }
    }
    inFile.close();
    outFile.close();
}

// Perform full decompression by building the tree and decoding the file
void huffman::decompress(){
    getTree();  // Rebuild the Huffman tree
    saveDecodedFile();  // Decode and write the file
}

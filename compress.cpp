#include "huff.hpp"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]){
    if(argc != 3){
        cout<<"Failed to detect files"<<endl;
        exit(1);
    }

    huffman h(argv[1], argv[2]);
    h.compress();
    cout<<"Compression successful"<<endl;
    return 0;
}

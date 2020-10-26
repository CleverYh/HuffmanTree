// #pragma pack(push)
// #pragma pack(1) //内存对其改为1个字节对齐模式

#include <iostream>
#include "fileCompress.h"
#include "fileDecompress.h"

using namespace std;

int main (int argc, char** argv) {
    handleFile test;
    test.toCompress("input.txt", "output.txt", "antitheses.txt");

    decodeFile test1;
    test1.decodeHuffmanTree("output.txt", "a.txt");
    test1.showHuffmanTree();

    return 0;
}            

// #pragma pack(pop)
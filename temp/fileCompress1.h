#include <iostream>
#include <Windows.h>
#include <cassert>

using namespace std;

struct charCounter {
    char _ch;
    int _count;
    string _code; // Huffman code

    charCounter (int count = 0) : _count(count) {} 
};

class handleFile  {
    
    charCounter _chars[256];

public:
    handleFile () {
        for (int i = 0; i < 256; i++) {
            _chars[i]._ch = i;
            _chars[i]._count = 0;
        }
    } 

    void toCompress (const char* filename) {
        FILE* fileOut = fopen(filename, "rb");
        assert(fileOut);
        char ch = fgetc(fileOut);
        while (!feof(fileOut)) {
            _chars[ch]._count++;
        }

        
    }
};
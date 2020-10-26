// #pragma pack(push)
// #pragma pack(1)

#include "fileCompress.h"
#include "fileDecompress.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv) {

    char *inputfile, *outputfile;

    // if (argc == 1) {
    //     int mode;
    //     cout << "Please select the mode" << endl
    //          << "compress -------- 0" << endl
    //          << "decompress ------ 1" << endl;
    //     cin >> mode;
    //     cout << "Please input the path of input file:";
    //     cin >> inputfile;
    //     cout << "Please input the path of output file:";
    //     cin >> outputfile;
    //     if (!mode) { // compress
    //         handleFile comp;
    //         comp.toCompress(inputfile, outputfile, "antitheses.txt");
    //     } else { // decompress
    //         decodeFile decomp;
    //         decomp.decodeHuffmanTree(inputfile, outputfile);
    //     }
    // } else
    if (argc == 3) {
        if (string(argv[1]) == "encode" || string(argv[1]) == "compress" || string(argv[1]) == "Compress") {
            handleFile comp;
            comp.toCompress(argv[2],"encodeout.txt","antitheses.txt");
        } else if (string(argv[1]) == "decompress" || string(argv[1]) == "Decompress" || string(argv[1]) == "decode") {
            decodeFile decomp;
            decomp.decodeHuffmanTree(argv[2],"decodeout.txt");
        } else {
            cout << "Please input valid command!" << endl;
        }
    } else if (argc == 4) {
        if (string(argv[1]) == "encode" || string(argv[1]) == "compress" || string(argv[1]) == "Compress") {
            handleFile comp;
            comp.toCompress(argv[2],argv[3],"antitheses.txt");
        } else if (string(argv[1]) == "decompress" || string(argv[1]) == "Decompress" || string(argv[1]) == "decode") {
            decodeFile decomp;
            decomp.decodeHuffmanTree(argv[2],argv[3]);
        } else {
            cout << "Please input valid command!" << endl;
        }
    } else {
        cout << "Please input valid command!" << endl;
    }

    // handleFile test;
    // test.toCompress("input.txt", "output.txt", "antitheses.txt");

    // decodeFile test1;
    // test1.decodeHuffmanTree("output.txt", "a.txt");

    return 0;
}

// #pragma pack(pop)
#ifndef __HUFFMAN_TREE_H__
#define __HUFFMAN_TREE_H__

#define DEBUG false
#define MAX_SIZE 100000000 // prevent endless loop
// get the bit of the index in a byte
#define GET_BYTE(_byte, _index) (((_byte) & (1 << ((_index) ^ 7))) != 0)
// set and clear
#define SET_BYTE(_byte, _index) ((_byte) |= (1 << ((_index) ^ 7)))
#define CLR_BYTE(_byte, _index) ((_byte) &= (~(1 << ((_index) ^ 7))))

// #pragma pack(push)
// #pragma pack(1)
#include <iostream>
#include <string>

using namespace std;

struct FILE_HEAD {
    unsigned char alphaVariety; // alpha variety
    // unsigned char sizeOfw; 
    unsigned char lastValidBit; // last valid bit
    // unsigned char unused[1];
};  // occupied 2 bytes

struct WEIGHT {
    unsigned char _ch;
    unsigned char _freq;
};

struct HuffmanTreeNode {
    //    HuffmanTreeNode* _father;
    HuffmanTreeNode *_left;
    HuffmanTreeNode *_right;
    int _weight;
    unsigned char _ch;

    HuffmanTreeNode(HuffmanTreeNode *left = nullptr, HuffmanTreeNode *right = nullptr, int weight = 0, unsigned char ch = 0) : _left(left), _right(right), _weight(weight), _ch(ch) {}
};

class cmp { // compare in priority_queue
public:
    bool operator()(const HuffmanTreeNode *left, const HuffmanTreeNode *right) {
        return left->_weight > right->_weight;
    }
};

// #pragma pack(pop)
#endif /* __HUFFMAN_TREE_H__ */
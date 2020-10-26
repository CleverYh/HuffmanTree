#ifndef __HUFFMAN_TREE_H__
#define __HUFFMAN_TREE_H__
// #pragma pack(push)
// #pragma pack(1) //内存对其改为1个字节对齐模式
#include <iostream>
#include <string>

using namespace std;

struct FILE_HEAD {
    unsigned char alphaVariety; //字符种类
    // unsigned char sizeOfw;    //w大小
    unsigned char lastValidBit; //最后一个字节的有效位数
    // unsigned char unused[1];    //待用空间
};                             //这个结构体总共占用4个字节的空间

struct HuffmanTreeNode {
    HuffmanTreeNode* _left;
    HuffmanTreeNode* _right;
//    HuffmanTreeNode* _father;
    int _weight;
    unsigned char _ch;

    HuffmanTreeNode (HuffmanTreeNode* left = nullptr, HuffmanTreeNode* right = nullptr, int weight = 0, unsigned char ch = 0) :
    _left(left), _right(right),  _weight(weight), _ch(ch) {}

};

struct WEIGHT{
    unsigned char _ch;
    unsigned char _freq;
};
class cmp{ // compare in priority_queue
public:
    bool operator() (const HuffmanTreeNode* left, const HuffmanTreeNode* right) {
        return left->_weight > right->_weight;
    } 
};

//得到index位的值，若index位为0，则GET_BYTE值为假，否则为真
#define GET_BYTE(vbyte, index) (((vbyte) & (1 << ((index) ^ 7))) != 0)
//index位置1
#define SET_BYTE(vbyte, index) ((vbyte) |= (1 << ((index) ^ 7)))
//index位置0
#define CLR_BYTE(vbyte, index) ((vbyte) &= (~(1 << ((index) ^ 7))))
// class HuffmanTree {
//     HuffmanTreeNode* root;

// public:
//     HuffmanTree() {}
//     HuffmanTree () {

//     }
// };
// #pragma pack(pop)
#endif  /* __HUFFMAN_TREE_H__ */
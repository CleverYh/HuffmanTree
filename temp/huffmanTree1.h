#include <iostream>
#include <string>
#include <queue>

using namespace std;

struct HuffmanTreeNode {
    HuffmanTreeNode* _left;
    HuffmanTreeNode* _right;
    HuffmanTreeNode* _father;

    unsigned char _ch;
    int _weight;

    HuffmanTreeNode (HuffmanTreeNode* left = nullptr, HuffmanTreeNode* right = nullptr, HuffmanTreeNode* father = nullptr, unsigned char ch = NULL,int weight = 0) :
    _left(left), _right(right), _father(father), _ch(ch), _weight(weight) {}

};

class HuffmanTree {
    HuffmanTreeNode* root;

public:
    HuffmanTree() {}
    HuffmanTree (   ) {
        priority_queue<HuffmanTreeNode, vector<HuffmanTreeNode>, greater<HuffmanTreeNode> > pq;

    }
};
#include <iostream>
#include <string>

using namespace std;

struct HuffmanTreeNode {
    HuffmanTreeNode* _left;
    HuffmanTreeNode* _right;
    HuffmanTreeNode* _father;
    int _weight;
    HuffmanTreeNode (HuffmanTreeNode* left = nullptr, HuffmanTreeNode* right = nullptr, HuffmanTreeNode* father = nullptr, int weight = 0) :
    _left(left), _right(right), _father(father), _weight(weight) {}

};

class HuffmanTree {
    HuffmanTreeNode* root;

public:
    HuffmanTree() {}
    HuffmanTree () {
        
    }
};
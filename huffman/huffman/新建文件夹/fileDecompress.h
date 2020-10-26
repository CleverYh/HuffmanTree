#ifndef __FILE_DECOMPRESS_H__
#define __FILE_DECOMPRESS_H__
// #pragma pack(push)
// #pragma pack(1) //内存对其改为1个字节对齐模式
#include "huffmanTree.h"
#include <Windows.h>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
using namespace std;

class decodeFile {

    struct FILE_HEAD fileHead;
    string str;
    struct WEIGHT *w;
    HuffmanTreeNode *_chars[256];
    HuffmanTreeNode *root;

public:
    void readFileHead(const char *sourceFileName, const char *outFileName) {
        cout << "Enter readFileHead" << endl;
        unsigned char buffer;
        FILE *fpIn = fopen(sourceFileName, "rb");
        fread(&fileHead, sizeof(struct FILE_HEAD), 1, fpIn); // read file head
        cout << (int)fileHead.sizeOfw << endl;
        cout << (int)fileHead.alphaVariety << endl;
        cout << (int)fileHead.lastValidBit << endl;
        // fseek(fpIn, 4, SEEK_SET); //略过前面4个字节的元数据
        // long curLocation = ftell(fpIn);
        int alphaVariety = (int)fileHead.alphaVariety;
        w = new struct WEIGHT[alphaVariety];
        fread(w, sizeof(struct WEIGHT), alphaVariety, fpIn);
        fclose(fpIn);
        // cout << (unsigned char)w[0]._ch << " " << w[0]._freq << endl;
        // fread(&buffer, sizeof(unsigned char), 1, fpIn);
    }

    void createHuffmanTree() {
        cout << "Enter createHuffmanTree" << endl;
        // start building huffman tree
        priority_queue<HuffmanTreeNode *, vector<HuffmanTreeNode *>, cmp> q;
        for (int i = 0; i < fileHead.alphaVariety; i++) {
            cout << i << endl;
            if (w[i]._freq != 0) {
                // cout << w[i]._ch << " " << w[i]._freq << endl;
                _chars[w[i]._ch] = new HuffmanTreeNode();
                _chars[w[i]._ch]->_ch = w[i]._ch;
                _chars[w[i]._ch]->_weight = w[i]._freq;
                q.push(_chars[w[i]._ch]);
            }
        }
        while (q.size() > 1) {
            HuffmanTreeNode *left = q.top();
            q.pop();
            HuffmanTreeNode *right = q.top();
            q.pop();
            HuffmanTreeNode *newHead = new HuffmanTreeNode(left, right, left->_weight + right->_weight, '\0');
            q.push(newHead);
        }
        // root = new HuffmanTreeNode;
        root = q.top();
        cout << "Exit createHuffmanTree" << endl;
    }

    void decodeHuffmanTree(const char *sourceFileName, const char *outFileName) {
        readFileHead(sourceFileName, outFileName);
        createHuffmanTree();
        cout << "Enter decodeHuffmanTree" << endl;
        unsigned char inValue;
        unsigned char outValue;
        long curLocation;
        FILE *fpIn = fopen(sourceFileName, "rb");
        FILE *fpOut = fopen(outFileName, "wb");
        fseek(fpIn, 0L, SEEK_END);
        long fileSize = ftell(fpIn); //文件总长度fileSize
        cout << "size: " << dec << fileSize << endl;
        // cout << sizeof(struct WEIGHT) * fileHead.alphaVariety;
        fseek(fpIn, 4 + sizeof(struct WEIGHT) * fileHead.alphaVariety, SEEK_SET); //略过前面4个字节的元数据，字符种类和频度
        curLocation = 4 + sizeof(struct WEIGHT) * fileHead.alphaVariety + 1;
        fread(&inValue, sizeof(unsigned char), 1, fpIn);
        cout <<"inValue:"<< hex << (int)inValue << endl;
        unsigned char index = 0;
        int i = 1000000; // TODO: Max while to be define
        HuffmanTreeNode *p = root;
        string debug;
        int flag = 0;
        while (i--) {
            cout<<i<<endl;
            // cout << "curLoca:"<<dec << curLocation << endl;
            if (p->_left == nullptr && p->_right == nullptr) {
                outValue = p->_ch;
                cout << "outValue:" << outValue << " " << curLocation << " " << index<<endl;
                fwrite(&outValue, sizeof(unsigned char), 1, fpOut);
                if(flag == 1)break;
                p = root;
            }
            

            //'1'向左子树走，'0'向右子树走
            //若超过一个字节，则需要读取下一个字节
            if (!GET_BYTE(inValue, index)) {
                p = p->_left;
                cout<<"left"<<endl;
                debug.push_back('0');
            } else {
                p = p->_right;
                debug.push_back('1');
            }
            // cout <<"index"<< index;
            index++;
            if (curLocation > fileSize || curLocation >= fileSize && index >= fileHead.lastValidBit) {
                cout << "break";
                flag = 1;
            }
            if (index >= 8) {
                index = 0;
                fread(&inValue, sizeof(unsigned char), 1, fpIn);
                cout <<"inValue"<< hex << (int)inValue << endl;
                curLocation++;
                // cout << "curLoca:" << curLocation << endl;
            }
        }

        fclose(fpIn);
        fclose(fpOut);
        cout << debug << endl;
        cout << "End decodeHuffmanTree" << endl;
    }

    void showHuffmanTree() {
        string s = "|";
        dfs(s,root);
    }

    void dfs(string &s, HuffmanTreeNode* root) {
        if (!root->_left && !root->_right) {
            printNode(s,root);
            return;
        }

        if (s,root->_left) {
            s.push_back('0');
            dfs(s,root->_left);
            s.pop_back();
        }

        if (s,root->_right) {
            s.push_back('1');
            dfs(s,root->_right);
            s.pop_back();
        }

        return;

        
    }

    void printNode(string &s,HuffmanTreeNode* Node){
            printf("%-5u %-4u %-5u %-4c",
                   Node->_left,
                   Node->_right,
                   Node->_weight,
                   Node->_ch);
            cout << s << endl;
    }
};
// #pragma pack(pop)
#endif /* __FILE_DECOMPRESS_H__ */
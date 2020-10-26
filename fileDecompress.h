#ifndef __FILE_DECOMPRESS_H__
#define __FILE_DECOMPRESS_H__
// #pragma pack(push)
// #pragma pack(1)
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
        unsigned char buffer;
        FILE *fpIn = fopen(sourceFileName, "rb");
        fread(&fileHead, sizeof(struct FILE_HEAD), 1, fpIn); // read file head
        // cout << (int)fileHead.alphaVariety << endl;
        // cout << (int)fileHead.lastValidBit << endl;
        int alphaVariety = (int)fileHead.alphaVariety;
        w = new struct WEIGHT[alphaVariety];
        fread(w, sizeof(struct WEIGHT), alphaVariety, fpIn); // read weight
        fclose(fpIn);
    }

    void createHuffmanTree() { // start building huffman tree
        priority_queue<HuffmanTreeNode *, vector<HuffmanTreeNode *>, cmp> q;
        for (int i = 0; i < fileHead.alphaVariety; i++) {
            if (w[i]._freq != 0) {
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
        root = q.top();
    }

    void decodeHuffmanTree(const char *sourceFileName, const char *outFileName) {
        readFileHead(sourceFileName, outFileName);
        createHuffmanTree();
        unsigned char inValue;
        unsigned char outValue;
        long curLocation;
        FILE *fpIn = fopen(sourceFileName, "rb");
        FILE *fpOut = fopen(outFileName, "wb");
        fseek(fpIn, 0L, SEEK_END);
        long fileSize = ftell(fpIn); // total length
        if (DEBUG) cout << "size: " << dec << fileSize << endl;
        fseek(fpIn, 2 + sizeof(struct WEIGHT) * fileHead.alphaVariety, SEEK_SET); // jump 2 bytes meta data and weight
        curLocation = 2 + sizeof(struct WEIGHT) * fileHead.alphaVariety + 1;
        fread(&inValue, sizeof(unsigned char), 1, fpIn);
        if (DEBUG) cout << "inValue:" << hex << (int)inValue << endl;
        unsigned char index = 0;
        int i = MAX_SIZE;
        HuffmanTreeNode *p = root;
        string debug;
        int flag = 0;
        while (i--) {
            if (p->_left == nullptr && p->_right == nullptr) {
                outValue = p->_ch;
                if (DEBUG) cout << "outValue:" << outValue << " " << curLocation << " " << index << endl;
                fwrite(&outValue, sizeof(unsigned char), 1, fpOut);
                if (flag == 1) break; // to prevent break before write TODO: code here could be more elegant
                p = root;
            }

            if (!GET_BYTE(inValue, index)) { // '0' goes left child
                p = p->_left;
                if (DEBUG) debug.push_back('0');
            } else { // '1' goes right child
                p = p->_right;
                if (DEBUG) debug.push_back('1');
            }

            index++;
            if (curLocation > fileSize || curLocation >= fileSize && index >= fileHead.lastValidBit) flag = 1;

            if (index >= 8) {
                index = 0;
                fread(&inValue, sizeof(unsigned char), 1, fpIn);
                if (DEBUG) cout << "inValue" << hex << (int)inValue << endl;
                curLocation++;
            }
        }

        if (DEBUG) cout << debug << endl;

        fclose(fpIn);
        fclose(fpOut);
    }

    /* --------- if (DEBUG) ----------- */ 
    void showHuffmanTree() {
        string s = "| ";
        dfs(s, root);
    }

    void dfs(string &s, HuffmanTreeNode *root) {
        if (!root->_left && !root->_right) {
            printNode(s, root);
            return;
        }

        if (s, root->_left) {
            s.push_back('0');
            dfs(s, root->_left);
            s.pop_back();
        }

        if (s, root->_right) {
            s.push_back('1');
            dfs(s, root->_right);
            s.pop_back();
        }

        return;
    }

    void printNode(string &s, HuffmanTreeNode *Node) {
        // printf("%-5u %-4u %-5u %-4c",
        //        Node->_left,
        //        Node->_right,
        //        Node->_weight,
        //        Node->_ch);
        if (DEBUG) cout << Node->_weight << " " << Node->_ch << s << endl;
    }
};
// #pragma pack(pop)
#endif /* __FILE_DECOMPRESS_H__ */
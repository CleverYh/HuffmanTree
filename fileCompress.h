#ifndef __FILE_COMPRESS_H__
#define __FILE_COMPRESS_H__
// #pragma pack(push)
// #pragma pack(1)
#include "huffmanTree.h"
#include <Windows.h>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
using namespace std;

// struct charCounter {
//     char _ch;
//     int _count;
//     string _code;

//     charCounter (int count = 0) : _count(count) {}

// };

class handleFile {

    HuffmanTreeNode *_chars[256];

public:
    handleFile() {
        for (int i = 0; i < 256; i++) {
            _chars[i] = new HuffmanTreeNode();
            _chars[i]->_ch = i;
            _chars[i]->_weight = 0;
        }
    }

    void myDFS(string &s, map<unsigned char, string> &m, HuffmanTreeNode *root) {
        if (!root->_left && !root->_right) {
            m[root->_ch] = s;
            return;
        }

        if (root->_left) {
            s.push_back('0');
            myDFS(s, m, root->_left);
            s.pop_back();
        }

        if (root->_right) {
            s.push_back('1');
            myDFS(s, m, root->_right);
            s.pop_back();
        }

        return;
    }

    void recordWeight(map<unsigned char, string> &m, HuffmanTreeNode *root) {
        if (!root->_left && !root->_right) {
            m[root->_ch] = '0';
            return;
        }
        string s;
        myDFS(s, m, root);
        return;
    }

    void toCompress(const char *filename, const char *outfilename, const char *antithesesName) {
        FILE *fileOut = fopen(filename, "rb");
        // assert(fileOut);
        unsigned char ch = fgetc(fileOut);
        while (!feof(fileOut)) {
            _chars[ch]->_weight++;
            ch = fgetc(fileOut);
        }

        // start building huffman tree
        // unsigned char* w = new unsigned char [256];

        priority_queue<HuffmanTreeNode *, vector<HuffmanTreeNode *>, cmp> q;
        for (int i = 0; i < 256; i++) {
            if (_chars[i]->_weight) {
                q.push(_chars[i]);
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
        HuffmanTreeNode *root = q.top();

        if (DEBUG) showHuffmanTree(root);

        // start recording weight
        map<unsigned char, string> m;
        recordWeight(m, root);
        int count = 0;
        ofstream fout2(antithesesName);
        for (int i = 0; i < 256; i++) {
            if (_chars[i]->_weight) {
                fout2 << (int)_chars[i]->_ch << " " << m[_chars[i]->_ch] << endl;
                count++;
            }
        }
        fout2.close();

        int index = 0;
        struct FILE_HEAD fileHead;
        struct WEIGHT *w = new struct WEIGHT[count];
        fileHead.alphaVariety = (unsigned char)count;
        // fileHead.sizeOfw = (unsigned char)sizeof(struct WEIGHT);
        for (int i = 0; i < 256; i++) {
            if (_chars[i]->_weight) {
                w[index]._ch = i;
                w[index]._freq = _chars[i]->_weight;
                if (DEBUG) cout << (unsigned char)i << _chars[i]->_weight << (unsigned char)w[index]._ch << w[index]._freq << endl;
                index++;
            }
        }

        // find the last valid bit
        int counter = 0;
        int bitIndex = 0;
        unsigned char temp = 0;
        FILE *fileOut2 = fopen(filename, "rb");
        ch = fgetc(fileOut2);
        while (!feof(fileOut2)) {
            string str = m[ch];
            for (int i = 0; i < str.length(); i++) {
                // temp <<= 1;
                // if (str[i] == '1')
                //     temp += 1;
                counter++;
                if (counter == 8) { // 8bits = 1byte
                    // fout << temp;
                    // code.push_back(temp);
                    counter = 0;
                    temp = 0;
                }
            }
            ch = fgetc(fileOut2);
        }
        fileHead.lastValidBit = counter == 0 ? 8 : (unsigned char)counter;
        fclose(fileOut2);

        // start writing code
        FILE *foutp = fopen(outfilename, "wb");
        // cout << (int)fileHead.sizeOfw << endl;
        if (DEBUG) cout << (int)fileHead.alphaVariety << endl
                        << (int)fileHead.lastValidBit << endl;
        fwrite(&fileHead, sizeof(struct FILE_HEAD), 1, foutp);
        fwrite(w, sizeof(struct WEIGHT), fileHead.alphaVariety, foutp);

        fileOut2 = fopen(filename, "rb");
        temp = 0;
        counter = 0;
        // ofstream fout(outfilename, ios::app);
        ch = fgetc(fileOut2);
        while (!feof(fileOut2)) {
            string str = m[ch];
            for (int i = 0; i < str.length(); i++) {
                temp <<= 1;
                if (DEBUG) cout << str[i];
                if (str[i] == '1')
                    temp += 1;
                counter++;
                if (counter == 8) {
                    if (DEBUG) cout << "hex" << hex << (int)temp << endl;
                    fwrite(&temp, sizeof(unsigned char), 1, foutp);
                    // fout << temp;
                    counter = 0;
                    temp = 0;
                }
            }
            ch = fgetc(fileOut2);
        }
        while (counter) {
            temp <<= 1;
            counter++;
            if (counter == 8) {
                if (DEBUG) cout << "hex" << hex << (int)temp << endl;
                fwrite(&temp, sizeof(unsigned char), 1, foutp);
                // fout << temp;
                counter = 0;
            }
        }

        fclose(foutp);
        fclose(fileOut2);

        return;
    }

    void showHuffmanTree(HuffmanTreeNode *root) {
        string s = "|";
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
        printf("%-5u %-4u %-5u %-4c",
               Node->_left,
               Node->_right,
               Node->_weight,
               Node->_ch);
        cout << s << endl;
    }
};
// #pragma pack(pop)
#endif /* __FILE_COMPRESS_H__ */
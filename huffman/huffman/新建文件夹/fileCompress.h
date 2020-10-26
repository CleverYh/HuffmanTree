#ifndef __FILE_COMPRESS_H__
#define __FILE_COMPRESS_H__
// #pragma pack(push)
// #pragma pack(1) //内存对其改为1个字节对齐模式
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

        showHuffmanTree(root);

        // start recording weight
        map<unsigned char, string> m;
        recordWeight(m, root);

        ofstream fout2(antithesesName);
        int count = 0;
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
        fileHead.sizeOfw = (unsigned char)sizeof(struct WEIGHT);
        for (int i = 0; i < 256; i++) {
            if (_chars[i]->_weight) {
                w[index]._ch = i;
                w[index]._freq = _chars[i]->_weight;
                cout << (unsigned char)i << _chars[i]->_weight << (unsigned char)w[index]._ch << w[index]._freq << endl;
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
        cout << (int)fileHead.sizeOfw << endl;
        cout << (int)fileHead.alphaVariety << endl;
        cout << (int)fileHead.lastValidBit << endl;
        fwrite(&fileHead, sizeof(struct FILE_HEAD), 1, foutp);
        fwrite(w, sizeof(struct WEIGHT), fileHead.alphaVariety, foutp);
        // fclose(foutp);

#if 0
        unsigned char *str;
        counter = 0;
        bitIndex = 0;
        temp = 0;
        //assert(fileOut2);
        ch = fgetc(fileOut2);
        while (!feof(fileOut2)) {
            cout << 1;
            str = (unsigned char *)m[ch].c_str();
            //把每个字符的哈夫曼编码一个一个过。
            //如果是字符'0'，就转换为二进制的0
            //如果是字符'1'，就转换为二进制的1
            for (int i = 0; str[i]; i++) {
                if ('0' == str[i]) {
                    //value为一个字节
                    //从第1位依次赋值，若大于八位（一个字节）了，就写入文件中
                    CLR_BYTE(temp, bitIndex);
                } else {
                    SET_BYTE(temp, bitIndex);
                }
                bitIndex++;
                if (bitIndex >= 8) {
                    bitIndex = 0;
                    fwrite(&temp, sizeof(unsigned char), 1, foutp);
                }
            }
            ch = fgetc(fileOut2);
        }
        //如果最后一次不满一个字节，依然需要写到文件中，注意：写入的最后一个字节可能会存在垃圾位
        if (bitIndex) {
            fwrite(&temp, sizeof(unsigned char), 1, foutp);
        }
#endif
        // counter = 0;
        // bitIndex = 0;
        fileOut2 = fopen(filename, "rb");
        temp = 0;
        counter = 0;
        // ofstream fout(outfilename, ios::app);
        ch = fgetc(fileOut2);
        while (!feof(fileOut2)) {
            string str = m[ch];
            cout << ch << "(ch)" << endl;
            cout << str << " " << endl;
            for (int i = 0; i < str.length(); i++) {
                temp <<= 1;
                cout << str[i];
                if (str[i] == '1')
                    temp += 1;
                counter++;
                if (counter == 8) {
                    cout << "hex" << hex << (int)temp << endl;
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
                cout << "hex" << hex << (int)temp << endl;
                fwrite(&temp, sizeof(unsigned char), 1, foutp);
                // fout << temp;
                counter = 0;
            }
        }
        // fout.close();
        // // cout << (int)fileHead.lastValidBit << endl;
        // while (counter) { // fullfill the last byte
        //     temp <<= 1;
        //     counter++;
        //     if (counter == 8) {
        //         fout << temp;
        //         // code.push_back(temp);
        //         counter = 0; // break;
        //     }
        // }

        // unsigned char *c = (unsigned char *)(code.c_str());
        // fwrite(c, sizeof(unsigned char), code.size(), foutp);

        // fout << code;
        // fout.close();
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
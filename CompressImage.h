//
// Created by xudas on 2017/3/30.
//

#ifndef TEST1_COMPRESSIMAGE_H
#define TEST1_COMPRESSIMAGE_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "InputFile.h"
#include "OutputFile.h"

using namespace std;
#pragma pack(2)
struct BMPHeader {
    short _;
    unsigned int size;
    short __;
    short ___;
    unsigned int offset;
};
struct BMPInfo {
    int biSize;
    unsigned int width;
    unsigned int height;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    int biClrUsed;
    int biClrImportant;
};

class CompressImage {
private:
    unsigned char** data = nullptr;
    char* header = nullptr;
    unsigned int length;
    int realWidth;
    BMPHeader bmpHeader;
    BMPInfo bmpInfo;

    int getLen(unsigned char d) {
        int result = 1;

        while (d > 1) {
            d /= 2;
            ++result;
        }
        return result;
    }

    void traceback(int n, vector<int> &s, int* l) {
        if (n == 0)
            return;
        traceback(n - l[n], s, l);
        s.push_back(n - l[n]);
    }

    unsigned char get(int pos) {
        --pos;
        int height = pos / bmpInfo.width;
        int width = pos % bmpInfo.width;
        if (height % 2 == 1) {
            width = bmpInfo.width - width - 1;
        }
        return data[height][width];
    }

    void set(int pos, unsigned char value) {
        --pos;
        int height = pos / bmpInfo.width;
        int width = pos % bmpInfo.width;
        if (height % 2 == 1) {
            width = bmpInfo.width - width - 1;
        }
        data[height][width] = value;
    }

public:
    void compress(char* filename) {
        if (length == 0) {
            cout << "please readfile before compress" << endl;
            return;
        }
        //动规
        int* dp = new int[length + 1]();
        dp[0] = 11;
        int* eachLen = new int[length + 1]();
        int* eachBitLen = new int[length + 1]();
        eachLen[0] = 0;
        eachBitLen[0] = 0;
        dp[0] = 0;
        for (int i = 1; i <= length; i++) {
            eachBitLen[i] = getLen(get(i));//计算像素点p需要的存储位数
            int bmax = eachBitLen[i];
            dp[i] = dp[i - 1] + bmax;
            eachLen[i] = 1;
            for (int j = 2; j <= i && j <= 256; j++) {
                if (bmax < eachBitLen[i - j + 1]) {
                    bmax = eachBitLen[i - j + 1];
                }

                if (dp[i] > dp[i - j] + j * bmax) {
                    dp[i] = dp[i - j] + j * bmax;
                    eachLen[i] = j;
                }
            }
            dp[i] += 11;
        }
        vector<int> lengths;
        traceback(length, lengths, eachLen);
        lengths.push_back(length);

        int pos = 1;
        for (int j = 1; j < lengths.size(); j++) {
            eachLen[j] = eachLen[lengths[j]];
            int bitLength = 0;
            for (int i = 0; i < eachLen[j]; ++i) {
                bitLength = max(eachBitLen[pos++], bitLength);
            }
            eachBitLen[j] = bitLength;
        }
        pos = 1;
        OutputFile file(filename);
        file.write(header, bmpHeader.offset, 1);
        for (int j = 1; j < lengths.size(); j++) {
            file.bitWrite(8, eachLen[j] - 1);
            file.bitWrite(3, eachBitLen[j] - 1);
            for (int i = 0; i < eachLen[j]; ++i) {
                file.bitWrite(eachBitLen[j], get(pos++));
            }
        }
        file.finish();
        delete eachBitLen;
        delete eachLen;
        delete dp;
        cout << "cmporess finish" << endl;

    }

    void unCompress(char* filename) {
        InputFile file(filename);
        //读入文件信息头
        file.read(&bmpHeader, sizeof(bmpHeader), 1);
        //读入位图信息头
        file.read(&bmpInfo, sizeof(bmpInfo), 1);
        length = bmpInfo.width * bmpInfo.height;
        //保存所有头信息
        file.seek(0, SEEK_SET);
        header = new char[bmpHeader.offset];
        file.read(header, bmpHeader.offset, 1);
        realWidth = (bmpInfo.width + 3) / 4 * 4;
        data = new unsigned char* [bmpInfo.height];
        for (int k = 0; k < bmpInfo.height; ++k) {
            data[k] = new unsigned char[realWidth];
        }
        int i = 1;
        while (i <= bmpInfo.height * bmpInfo.width) {
            int len = file.bitRead(8) + 1;
            unsigned char bitLen = file.bitRead(3) + 1;
            for (int j = 0; j < len; ++j) {
                unsigned char temp = file.bitRead(bitLen);
                set(i++, temp);
            }
        }
        file.finish();
        cout << "uncmporess finish" << endl;
    }

    void writeFile(char* filename) {
        if (length == 0) {
            cout << "Nothing to write,do you uncompress or readfile before writefile?" << endl;
            return;
        }
        OutputFile file(filename);
        file.write(header, bmpHeader.offset, 1);
        for (int i = 0; i < bmpInfo.height; ++i) {
            file.write(data[i], realWidth, 1);
        }
        file.finish();
        cout << "writefile finish" << endl;
    }

    void readFile(char* filename) {
        //初始化
        InputFile file(filename);
        //读入文件信息头
        file.read(&bmpHeader, sizeof(bmpHeader), 1);
        //读入位图信息头
        file.read(&bmpInfo, sizeof(bmpInfo), 1);
        length = bmpInfo.width * bmpInfo.height;
        //保存所有头信息
        file.seek(0, SEEK_SET);
        header = new char[bmpHeader.offset];
        file.read(header, bmpHeader.offset, 1);
        realWidth = (bmpInfo.width + 3) / 4 * 4;
        data = new unsigned char* [bmpInfo.height];
        //读取所有像素
        for (int k = 0; k < bmpInfo.height; ++k) {
            data[k] = new unsigned char[realWidth];
            file.read(data[k], realWidth, 1);
        }
        cout << "readfile finish" << endl;
    }

    ~CompressImage(){
        delete data;
        delete header;
    }
};


#endif //TEST1_COMPRESSIMAGE_H
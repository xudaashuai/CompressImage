//
// Created by xudas on 2017/4/1.
//

#ifndef COMPRESSIMAGE_XIP_H
#define COMPRESSIMAGE_XIP_H


#include "InputFile.h"
#include <iostream>
#include <cstdlib>

using namespace std;

class Xip {
    struct Node{
        int count=0;
        unsigned char data;
        int deep=0;
    };
    static int cmp(const void* left, const void* right){
        Node* le=(Node*)left;
        Node* ri=(Node*)right;
        return le->count-ri->count;
    }
    struct CharZip{
        int length;
        int data;
    };
    class MaxHeap {
        int length=0;
        Node* data;
    public:
        MaxHeap(const Node* temp,int length):length(length){
            data=new Node[length];
            memcpy(data,temp, sizeof(Node)*length);
            qsort(data,256,sizeof(Node),cmp);
        }

    };
public:
    static void compress(char* filename){
        Node charCount[256];
        for (int l = 0; l < 256; ++l) {
            charCount[l].data=l;
        }
        InputFile inputFile(filename);
        int length =inputFile.length();
        unsigned char temp[1024];
        for (int i = 0; i < length/1024; ++i) {
            inputFile.read(temp,1,1024);
            for (int j = 0; j < 1024; ++j) {
                ++charCount[temp[j]].count;
            }
        }
        if (length%1024!=0){
            inputFile.read(temp,1,length%1024);
            for (int j = 0; j < length%1024; ++j) {
                ++charCount[temp[j]].count;
            }
        }
        for (int k = 0; k < 256; ++k) {
            cout<<charCount[k].count<<" "<<int(charCount[k].data)<<endl;
        }
        qsort(charCount,256,sizeof(Node),cmp);
        for (int k = 0; k < 256; ++k) {
            cout<<charCount[k].count<<" "<<int(charCount[k].data)<<endl;
        }
        CharZip chars[256];

    }
    static void unCompress(char* filename){

    }
};


#endif //COMPRESSIMAGE_XIP_H

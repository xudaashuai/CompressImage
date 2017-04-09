//
// Created by xudas on 2017/3/30.
//
/*
 * File with bit read
 */
#ifndef TEST1_INPUTFILE_H
#define TEST1_INPUTFILE_H


#include <cstdio>
#include "File.h"

class InputFile : public File {
private:
public:
    InputFile(char* filename) : File(filename, "rb", 0, 0) {}

    unsigned int bitRead(int bitLength) {
        unsigned int result = 0;
        if (cacheLength < bitLength) {
            result = (cache >> (32 - cacheLength)) << (bitLength -= cacheLength);
            fread(&cache, sizeof(int), 1, file);
            cacheLength = 32;
        }
        result |= cache >> (32 - bitLength);
        cache <<= bitLength;
        cacheLength -= bitLength;
        return result;
    }

    void read(void* data, size_t size, size_t count) {
        fread(data, size, count, file);
    }
};


#endif //TEST1_INPUTFILE_H

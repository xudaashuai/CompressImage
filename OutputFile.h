//
// Created by xudas on 2017/3/30.
//
/*
 * File with bit write
 */
#ifndef TEST1_OUTPUTFILE_H
#define TEST1_OUTPUTFILE_H


#include <cstdio>

class OutputFile:public File {
public:
    OutputFile(char* filename):File(filename,"wb+",0,32){}
    void write(const void* data,size_t size,size_t count){
        flush();
        fwrite(data,size,count,file);
        flush();
    }
    void bitWrite(int bitLength,unsigned int data){
        if (cacheLength<bitLength){
            cache<<=cacheLength;
            cache|=data>>(bitLength-=cacheLength);
            fwrite(&cache,1,sizeof(cache),file);
            cache=0;
            cacheLength=32;
        }
        cache<<=bitLength;
        cache|=data<<32-bitLength>>32-bitLength;
        cacheLength-=bitLength;
    }
    void flush(){
        if (cacheLength!=32){
            cache<<=cacheLength;
            fwrite(&cache,sizeof(cache),1,file);
        }
        fflush(file);
    }
    void finish(){
        flush();
        File::finish();
    }
};


#endif //TEST1_OUTPUTFILE_H

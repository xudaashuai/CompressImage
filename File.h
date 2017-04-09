//
// Created by xudas on 2017/3/30.
//
/*
 * Base class of InputFile and OutputFile
 */
#ifndef TEST1_FILE_H
#define TEST1_FILE_H


#include <cstdio>

class File {
protected:
    FILE *file;
    unsigned int cache;
    int cacheLength;
    long len;
public:
    File(char* filename,char* mode,unsigned int cache,int cacheLength):cacheLength(cacheLength),cache(cache){
        file=fopen(filename,mode);
        fseek(file, 0, SEEK_END);
        len= ftell(file);
        fseek(file, 0, SEEK_SET);
    }
    virtual void finish(){
        fflush(file);
        fclose(file);
    }
    void seek(long offset,int pos){
        fseek(file,offset,pos);
    }
    long length() {
        return len;
    }
    ~File(){
        finish();
    }
};


#endif //TEST1_FILE_H

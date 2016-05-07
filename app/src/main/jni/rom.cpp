//
// Created by Cauchywei on 16/5/7.
//

#include "Rom.h"
#include <cstring>

Rom::Rom(const char *path){

    FILE *pFILE = fopen(path, "r");

    if (!pFILE) {
        return ;
    }

    int fileSize = fseek(pFILE, 0, SEEK_END);

    content = new byte[fileSize];
    byte* contentP = content;

    const int BUFFER_SIZE = 1024;
    byte buffer[BUFFER_SIZE];
    int count;

    while((count = fread(buffer,BUFFER_SIZE, sizeof(byte),pFILE)) != 0){
        memccpy(contentP,buffer,count, sizeof(byte));
        contentP += count;
    }

    fclose(pFILE);
}
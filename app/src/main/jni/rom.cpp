//
// Created by Cauchywei on 16/5/7.
//

#include "ROM.h"

ROM::ROM(const char *path){

    FILE *pFILE = fopen(path, "r");

    if (!pFILE) {
        return ;
    }

    int fileSize = fseek(pFILE, 0, SEEK_END);

    content = new byte[fileSize];
    byte* contentP = content;

    const int BUFFER_SIZE = 1024;
    byte buffer[BUFFER_SIZE];
    size_t count;

    while((count = fread(buffer,BUFFER_SIZE, sizeof(byte),pFILE)) != 0){
        memcpy(contentP,buffer,count);
        contentP += count;
    }

    fclose(pFILE);
}

void ROM::load() {

    valid = false;

    if (content == nullptr) {
        return;
    }

    auto fileSize = sizeof(content) / sizeof(byte);

    if (fileSize < 16) {
        return;
    }

    //NES file start with "NES\x1a"
    if(content[0] != 'N' || content[1] != 'E' || content[2] != 'S' || content[1] != '\u001a') {
        return;
    }

    romBankCount = content[4];
    vromBankCount = content[5];
    mirrorType = (byte) (content[6] & 1); // HORIZONTAL_MIRRORING : VERTICAL_MIRRORING;
    hasBatteryRam = ((content[6] & 0b10) >> 1) == 1;
    hasTrainer = ((content[6] & 0b100) >> 2) == 1;
    hasFourScreen = ((content[6] & 0b1000) >> 3) == 1;
    romMapperType = (byte) (((content[6] & 0xF0) >> 4) | (content[7] & 0xF0));

    if (hasBatteryRam) {
        //TODO
    }


    //Lower 4 bit  $7 must be 0
    if (((content[7] & 0xF) != 0)) {
        return;
    }

    //Lower 4 bit int content[7] must be 0
    for (auto i = 8; i < 0xF; i++) {
        if (content[i] != 0) {
            return;
        }
    }

    const auto romUnitSize = 16 * 1024;

    int prgRomSize = romUnitSize * romBankCount;

    if(fileSize - HEADER_LENGTH < prgRomSize) {
        return;
    }

    //load Program Rom from file
    rom = new byte*[romBankCount];
    byte* p = content;
    p += HEADER_LENGTH;

    for (auto i = 0; i < romBankCount; i++) {
        rom[i] = new byte[romUnitSize];
        memcpy(rom[i], p, romUnitSize);
        p += romUnitSize;
    }

    const auto vromBankSize = 8 * 1024;
    int chrRomSize = romUnitSize * vromBankCount;

    if(fileSize - HEADER_LENGTH - prgRomSize < chrRomSize) {
        return;
    }

    //load Character Rom from file
    vrom = new byte*[vromBankCount];
    for (int i = 0; i < vromBankSize; i++) {
        vrom[i] = new byte[vromBankSize];
        memcpy(vrom[i], p, vromBankSize);
        p += vromBankSize;
    }





    valid  = true;
}

ROM::~ROM() {

    delete this->content;

    if (rom != nullptr) {
        size_t len = sizeof(rom) / sizeof(byte*);
        for (size_t i = 0; i < len; ++i) {
            delete [] rom[i];
        }
    }

    if (vrom != nullptr) {
        size_t len = sizeof(vrom) / sizeof(byte*);
        for (size_t i = 0; i < len; ++i) {
            delete [] vrom[i];
        }
    }
}

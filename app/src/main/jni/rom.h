//
// Created by Cauchywei on 16/5/7.
//

#ifndef NESDROID_ROM_H
#define NESDROID_ROM_H

#include <cstdint>
#include <cstdio>
#include <map>

#include "commons.h"

using namespace std;

static const int HEADER_LENGTH = 0xf;

static const uint8_t HORIZONTAL_MIRRORING = 0;
static const uint8_t VERTICAL_MIRRORING = 1;
static const uint8_t FOUR_SCREEN_MIRRORING = 2;

static map<uint16_t, const char *> MAPPER_NAMES = {
        {1,  "Nintendo MMC1"},
        {2,  "UNROM"},
        {3,  "CNROM"},
        {4,  "Nintendo MMC3"},
        {5,  "Nintendo MMC5"},
        {6,  "FFE F4xxx"},
        {7,  "AOROM"},
        {8,  "FFE F3xxx"},
        {9,  "Nintendo MMC2"},
        {10, "Nintendo MMC4"},
        {11, "Color Dreams Chip"},
        {12, "FFE F6xxx"},
        {15, "100-in-1 switch"},
        {16, "Bandai chip"},
        {17, "FFE F8xxx"},
        {18, "Jaleco SS8806 chip"},
        {19, "Namcot 106 chip"},
        {20, "Famicom Disk System"},
        {21, "Konami VRC4a"},
        {22, "Konami VRC2a"},
        {23, "Konami VRC2a"},
        {24, "Konami VRC6"},
        {25, "Konami VRC4b"},
        {32, "Irem G-101 chip"},
        {33, "Taito TC0190/TC0350"},
        {34, "32kB ROM switch"},
        {64, "Tengen RAMBO-1 chip"},
        {65, "Irem H-3001 chip"},
        {66, "GNROM switch"},
        {67, "SunSoft3 chip"},
        {68, "SunSoft4 chip"},
        {69, "SunSoft5 FME-7 chip"},
        {71, "Camerica chip"},
        {78, "Irem 74HC161/32-based"},
        {91, "Pirate HK-SF3 chip"}
};

//


class ROM {

public:

    ROM(byte *content) : content(content) { }

    ROM(const char *path);

    virtual ~ROM();

    void load();

    bool isValid() const {
        return valid;
    }

    inline const byte *const getContent() const {
        return content;
    }


    byte getRomCount() const {
        return romBankCount;
    }

    byte getVromCount() const {
        return vromBankCount;
    }

    byte getMirrorType() const {
        return mirrorType;
    }

    bool isHasBatteryRam() const {
        return hasBatteryRam;
    }

    bool isHasTrainer() const {
        return hasTrainer;
    }

    bool isHasFourScreen() const {
        return hasFourScreen;
    }

    byte getRomMapperType() const {
        return romMapperType;
    }

    const char* getMapperName(){
        const char* name;
        if(romMapperType < 0 || romMapperType > 92 || (name = MAPPER_NAMES[romMapperType]) == nullptr){
            return "No Mapper Name";
        } else{
            return name;
        }
    }

    byte **getPrgRom() const {
        return rom;
    }

    byte **getChrRom() const {
        return vrom;
    }

private:
    bool valid = false;
    byte *content = nullptr;

    byte romBankCount;
    byte vromBankCount;
    byte mirrorType;
    bool hasBatteryRam;
    bool hasTrainer;
    bool hasFourScreen;
    byte romMapperType;

    byte **rom = nullptr;
    byte **vrom = nullptr;
};


#endif //NESDROID_ROM_H

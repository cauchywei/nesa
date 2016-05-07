//
// Created by Cauchywei on 16/5/7.
//

#ifndef NESDROID_ROM_H
#define NESDROID_ROM_H

#include <cstdint>
#include <cstdio>
#include "commons.h"

class Rom {

public:

    Rom(byte* content):content(content) { }
    Rom(const char *path);


    bool isValid() const {
        return valid;
    }

private:
    bool valid = false;
    byte *content = nullptr;
};


#endif //NESDROID_ROM_H

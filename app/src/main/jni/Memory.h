//
// Created by Cauchywei on 16/5/14.
//

#ifndef NESDROID_MEMORY_H
#define NESDROID_MEMORY_H

#include "commons.h"

namespace nesdroid {


    class IMemory {
    public:
        virtual byte read(addr_t address) = 0;

        virtual dbyte readDoubleByte(addr_t address) = 0;

        virtual void writeDoubleByte(addr_t address, dbyte value) = 0;

        virtual void write(addr_t address, byte value) = 0;
    };

    class IMapper : public IMemory {

    };

    const

    class CpuMemory : public IMemory {

    public:

        friend class Cpu;

        CpuMemory() {
            ram = new byte[0x2000];
        }


        virtual ~CpuMemory() {
            delete ram;
            delete mapper;
        }

        virtual byte read(addr_t address) override;

        virtual void write(addr_t address, byte value) override;

        virtual dbyte readDoubleByte(addr_t address) override;

        virtual void writeDoubleByte(addr_t address, dbyte value) override;



    private:
        IMapper *mapper;
        byte *ram;

    public:

        void reset();

        dbyte readDoubleByteBugly(addr_t address);
    };
}
#endif //NESDROID_MEMERY_H

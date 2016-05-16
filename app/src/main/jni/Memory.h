//
// Created by Cauchywei on 16/5/14.
//

#ifndef NESDROID_MEMORY_H
#define NESDROID_MEMORY_H

#include "commons.h"

namespace nesdroid {

    static const byte INIT_SP = 0xFD;

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

        void push(byte value);

        void pushDoubleByte(dbyte value);

        byte pop();

        dbyte popDoubleByte();

        byte getFlags();

        void setFlags(byte flags);

    private:
        IMapper *mapper;
        byte *ram;
        ///////////Registers///////////
        byte ACC;

        byte X;
        byte Y;

        //Stack Pointer, 8bit, it decrease when push
        //Stack at memory locations $0100-$01FF
        byte SP = INIT_SP;

        //Program Counter, 16bit, point next instruction address
        addr_t PC;

        //Processor Status
        // 7 6 5 4 3 2 1 0
        // N V   B D I Z C
        bit CF; //Carry Flag
        bit ZF; //Zero Flag
        bit IF; //Interrupt Disable
        bit DF; //Decimal Mode
        bit BF; //Break Command
        //  --; //Empty
        bit VF; //Overflow Flag
        bit NF; //Negative Flag
    public:

        void reset();
    };
}
#endif //NESDROID_MEMERY_H

//
// Created by Cauchywei on 16/5/14.
//

#include "memory.h"

namespace nesdroid {

    byte CpuMemory::read(addr_t address) {

        if (address < 0x2000) {
            return ram[address % 0x0800];
        } else if (address < 0x4000) {
            //TODO read ppu register
        } else if (address < 0x4014) {
            //TODO read APU register
        } else if (address == 0x4014) {
            //TODO read PPU register
        } else if (address == 0x4015) {
            //TODO read APU register
        } else if (address == 0x4016) {
            //TODO read contoller
        } else if (address == 0x4017) {
            //TODO read APU register
        } else if (address < 0x6000) {
            // TODO: I/O registers
        } else {
            return mapper->read(address);
        }

    }


    dbyte CpuMemory::readDoubleByte(addr_t address) {
        if (address < 0x2000) {
            return *((dbyte *) (ram + address % 0x0800));
        } else if (address < 0x4000) {
            //TODO read ppu register
        } else if (address < 0x4014) {
            //TODO read APU register
        } else if (address == 0x4014) {
            //TODO read PPU register
        } else if (address == 0x4015) {
            //TODO read APU register
        } else if (address == 0x4016) {
            //TODO read contoller
        } else if (address == 0x4017) {
            //TODO read APU register
        } else if (address < 0x6000) {
            // TODO: I/O registers
        } else {
            return mapper->readDoubleByte(address);
        }

        return 0;
    }

    void CpuMemory::write(addr_t address, byte value) {

        if (address < 0x2000) {
            ram[address % 0x0800] = value;
        } else if (address < 0x4000) {
            //TODO write ppu register
        } else if (address < 0x4014) {
            //TODO write APU register
        } else if (address == 0x4014) {
            //TODO write PPU register
        } else if (address == 0x4015) {
            //TODO write APU register
        } else if (address == 0x4016) {
            //TODO write contoller
        } else if (address == 0x4017) {
            //TODO write APU register
        } else if (address < 0x6000) {
            // TODO: I/O registers
        } else {
            mapper->write(address, value);
        }
    }


    void CpuMemory::writeDoubleByte(addr_t address, dbyte value) {
        if (address < 0x2000) {
            *((dbyte *) (ram + address % 0x0800)) = value;
        } else if (address < 0x4000) {
            //TODO write ppu register
        } else if (address < 0x4014) {
            //TODO write APU register
        } else if (address == 0x4014) {
            //TODO write PPU register
        } else if (address == 0x4015) {
            //TODO write APU register
        } else if (address == 0x4016) {
            //TODO write contoller
        } else if (address == 0x4017) {
            //TODO write APU register
        } else if (address < 0x6000) {
            // TODO: I/O registers
        } else {
            mapper->writeDoubleByte(address, value);
        }
    }


    byte CpuMemory::getProcessorStatus() {
        return (byte) (CF << 7 || ZF << 6 || IF << 5 || DF << 4 || BF << 3 || VF << 1 || NF);
    }

    void CpuMemory::setProcessorStatus(byte flags) {
        CF = flags >> 7;
        ZF = flags >> 6;
        IF = flags >> 5;
        DF = flags >> 4;
        BF = flags >> 4;
        VF = flags >> 2;
        NF = flags >> 1;
    }


    void CpuMemory::push(byte value) {
        ram[SP-- | STACK_BASE] = value;
    }

    void CpuMemory::pushDoubleByte(dbyte value) {
        push((byte) (value >> 8));
        push((byte) value);
    }

    byte CpuMemory::pop() {
        return ram[++SP | STACK_BASE];
    }

    dbyte CpuMemory::popDoubleByte() {
        byte low = pop();
        byte high = pop();
        return (high << 8) | low;
    }

    void CpuMemory::reset() {

    }

    void CpuMemory::onResetInterrupt() {
        PC = readDoubleByte(0xFFFC);
    }

    //IRQ
    void CpuMemory::onMaskableInterrupt() {
        pushDoubleByte(PC);
        push(getProcessorStatus());
        PC = readDoubleByte(0xFFFE);
        IF = 1;
    }

    void CpuMemory::onNonMaskableInterrupt() {
        pushDoubleByte(PC);
        push(getProcessorStatus());
        PC = readDoubleByte(0xFFFA);
        IF = 1;
    }

    // readDoubleByteBugly emulates a 6502 bug that caused the low byte to wrap without
    // incrementing the high byte
    dbyte CpuMemory::readDoubleByteBugly(addr_t address) {
        addr_t a = address;
        addr_t b = (addr_t) ((address & 0xFF00) | (((byte) a) + 1));
        byte low = read(a);
        byte high = read(b);
        return high << 8 | low;
    }
}
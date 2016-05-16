//
// Created by Cauchywei on 16/5/7.
//

#include "cpu.h"

namespace nesdroid{

    void Cpu::onResetInterrupt() {
         memory->reset();
    }

    void Cpu::onMaskableInterrupt() {

    }

    void Cpu::onNonMaskableInterrupt() {

    }



    uint64_t Cpu::excuse() {

        if(stallCycle > 0 ){
            --stallCycle;
            return 1;
        }


        switch (interrupt) {

            case NONE:break;
            case MASKABLE_INTERUPT:
                onMaskableInterrupt();
                break;
            case NON_MASKABLE_INTERUPT:
                onNonMaskableInterrupt();
                break;
            case RESET:
                onResetInterrupt();
                break;
        }

        interrupt = NONE;


        byte optCode = memory->read(PC);

        const char *name = InstructionNameTable[optCode];
        opt const pOperation = InstructionTable[optCode];
        byte cycle = InstructionCycleTable[optCode];
        byte length = InstructionLengthTable[optCode];
        byte pageCycle = InstructionPageCycleTable[optCode];
        byte addressingMode = AddressingModeTable[optCode];

        if (pOperation == nullptr) {


        }
        
        return 0;
    }


}

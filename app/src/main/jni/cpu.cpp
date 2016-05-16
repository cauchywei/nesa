//
// Created by Cauchywei on 16/5/7.
//

#include "cpu.h"

namespace nesdroid {

    // pagesDiffer returns true if the two addresses reference different pages
    static bool pagesDiffer(const addr_t &a, const addr_t &b) const {
        return (a & 0xFF00) != (b & 0xFF00);
    }

    void Cpu::onResetInterrupt() {
        memory->onResetInterrupt();
    }

    void Cpu::onMaskableInterrupt() {
        memory->onMaskableInterrupt();
        cycles += 7;
    }

    void Cpu::onNonMaskableInterrupt() {
        memory->onNonMaskableInterrupt();
        cycles += 7;
    }




    uint64_t Cpu::excuse() {

        if (stallCycle > 0) {
            --stallCycle;
            return 1;
        }


        switch (interrupt) {

            case NONE:
                break;
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


        byte optCode = memory->read(memory->PC);

        const char *name = InstructionNameTable[optCode];
        opt const pOperation = InstructionTable[optCode];
        byte cycle = InstructionCycleTable[optCode];
        byte instructionLength = InstructionLengthTable[optCode];
        byte pageCycle = InstructionPageCycleTable[optCode];
        AddressingMode addressingMode = AddressingModeTable[optCode];

        if (pOperation == nullptr) {
            LOG("Illegal OptCode %s:%d\n", name, optCode);
//            return this->cycles;
        }

        addr_t address = 0;
        bool pageCrossed = false;
        addr_t nextPC = (addr_t) (memory->PC + 1);
        switch (addressingMode) {
            case ZERO_PAGE:
                address = memory->read(nextPC);
                break;
            case ZERO_PAGE_X:
                address = memory->read(nextPC) + memory->X;
                break;
            case ZERO_PAGE_Y:
                address = memory->read(nextPC) + memory->Y;
                break;
            case ABSOLUTE:
                address = memory->readDoubleByte(nextPC);
                break;
            case ABSOLUTE_X:
                address = memory->readDoubleByte(nextPC) + memory->X;
                pageCrossed = pagesDiffer(address-memory->X,address);
                break;
            case ABSOLUTE_Y:
                address = memory->readDoubleByte(nextPC) + memory->Y;
                pageCrossed = pagesDiffer(address-memory->Y,address);
                break;
            case INDIRECT:
                address = memory->readDoubleByteBugly(memory->readDoubleByte(nextPC));
                break;
            case IMPLIED:
                break;
            case ACCUMULATOR:
                break;
            case IMMEDIATE:
                address = nextPC;
                break;
            case RELATIVE:
                dbyte offset = memory->readDoubleByte(nextPC);
                address = (addr_t) (memory->PC + 2 + offset + (offset < 0x80 ? 0 : -0x100));
                break;
            case INDEXED_INDIRECT:
                address = memory->readDoubleByteBugly(memory->readDoubleByte(nextPC) + memory->X);
                break;
            case INDIRECT_INDEXED:
                address = memory->readDoubleByteBugly(memory->readDoubleByte(nextPC)) + memory->Y;
                pageCrossed = pagesDiffer(address-memory->Y,address);
                break;
            default:
                LOG("Error AddressingMode %d", addressingMode);
        }


        this->memory->PC += instructionLength;
        this->cycles += cycle;

        if (pageCrossed) {
            this->cycles = pageCycle;
        }

        if (pOperation != nullptr) {
            this->(*pOperation)({address,memory->PC,addressingMode});
        }

        return this->cycles - cycle;
    }


    void Cpu::ADC(const Context &context) {

    }

    void Cpu::AND(const Context &context) {

    }

    void Cpu::ASL(const Context &context) {

    }

    void Cpu::BCC(const Context &context) {

    }

    void Cpu::BCS(const Context &context) {

    }

    void Cpu::BEQ(const Context &context) {

    }

    void Cpu::BIT(const Context &context) {

    }

    void Cpu::BMI(const Context &context) {

    }

    void Cpu::BNE(const Context &context) {

    }

    void Cpu::BPL(const Context &context) {

    }

    void Cpu::BRK(const Context &context) {

    }

    void Cpu::BVC(const Context &context) {

    }

    void Cpu::BVS(const Context &context) {

    }

    void Cpu::CLC(const Context &context) {

    }

    void Cpu::CLD(const Context &context) {

    }

    void Cpu::CLI(const Context &context) {

    }

    void Cpu::CLV(const Context &context) {

    }

    void Cpu::CMP(const Context &context) {

    }

    void Cpu::CPX(const Context &context) {

    }

    void Cpu::CPY(const Context &context) {

    }

    void Cpu::DEC(const Context &context) {

    }

    void Cpu::DEX(const Context &context) {

    }

    void Cpu::DEY(const Context &context) {

    }

    void Cpu::EOR(const Context &context) {

    }

    void Cpu::INC(const Context &context) {

    }

    void Cpu::INX(const Context &context) {

    }

    void Cpu::INY(const Context &context) {

    }

    void Cpu::JMP(const Context &context) {

    }

    void Cpu::JSR(const Context &context) {

    }

    void Cpu::LDA(const Context &context) {

    }

    void Cpu::LDX(const Context &context) {

    }

    void Cpu::LDY(const Context &context) {

    }

    void Cpu::LSR(const Context &context) {

    }

    void Cpu::NOP(const Context &context) {

    }

    void Cpu::ORA(const Context &context) {

    }

    void Cpu::PHA(const Context &context) {

    }

    void Cpu::PHP(const Context &context) {

    }

    void Cpu::PLA(const Context &context) {

    }

    void Cpu::PLP(const Context &context) {

    }

    void Cpu::ROL(const Context &context) {

    }

    void Cpu::ROR(const Context &context) {

    }

    void Cpu::RTI(const Context &context) {

    }

    void Cpu::RTS(const Context &context) {

    }

    void Cpu::SBC(const Context &context) {

    }

    void Cpu::SEC(const Context &context) {

    }

    void Cpu::SED(const Context &context) {

    }

    void Cpu::SEI(const Context &context) {

    }

    void Cpu::STA(const Context &context) {

    }

    void Cpu::STX(const Context &context) {

    }

    void Cpu::STY(const Context &context) {

    }

    void Cpu::TAX(const Context &context) {

    }

    void Cpu::TAY(const Context &context) {

    }

    void Cpu::TSX(const Context &context) {

    }

    void Cpu::TXA(const Context &context) {

    }

    void Cpu::TXS(const Context &context) {

    }

    void Cpu::TYA(const Context &context) {

    }
}

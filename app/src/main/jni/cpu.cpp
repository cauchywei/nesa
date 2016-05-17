//
// Created by Cauchywei on 16/5/7.
//

#include "cpu.h"

namespace nesdroid {

    // pagesDiffer returns true if the two addresses reference different pages
    static inline bool pagesDiffer(const addr_t &a, const addr_t &b) const {
        return (a & 0xFF00) != (b & 0xFF00);
    }


    void Cpu::pcGoto(const Context &context) {
        PC = context.address;
        cycles++;

        // addBranchCycles adds a cycle for taking a branch and adds another cycle
        // if the branch jumps to a new page
        if (pagesDiffer(context.PC, context.address)) {
            cycles++;
        }
    }


    void Cpu::compare(byte a, byte b) {
        setZN(a - b);
        CF = a >= b;
    }

    void Cpu::onResetInterrupt() {
        PC = memory.readDoubleByte(0xFFFC);
        //TODO
    }

    void Cpu::onMaskableInterrupt() {
        pushDoubleByte(PC);
        push(getProcessorStatus());
        PC = memory.readDoubleByte(0xFFFE);
        IF = 1;
        cycles += 7;
    }

    void Cpu::onNonMaskableInterrupt() {
        pushDoubleByte(PC);
        push(getProcessorStatus());
        PC = memory.readDoubleByte(0xFFFA);
        IF = 1;
        cycles += 7;
    }


    byte Cpu::getProcessorStatus() {
        return (byte) (CF << 7 || ZF << 6 || IF << 5 || DF << 4 || BF << 3 || VF << 1 || NF);
    }

    void Cpu::setProcessorStatus(byte flags) {
        CF = flags >> 7;
        ZF = flags >> 6;
        IF = flags >> 5;
        DF = flags >> 4;
        BF = flags >> 4;
        VF = flags >> 2;
        NF = flags >> 1;
    }

    void Cpu::setZN(const byte &value) {
        NF = (value & 0x80) == 1;
        ZF = value == 0;
    }

    void Cpu::push(byte value) {
        memory.write(SP-- | STACK_BASE, value);
    }

    void Cpu::pushDoubleByte(dbyte value) {
        push((byte) (value >> 8));
        push((byte) value);
    }

    byte Cpu::pull() {
        return memory.read(++SP | STACK_BASE);
    }

    dbyte Cpu::pullDoubleByte() {
        byte low = pull();
        byte high = pull();
        return (high << 8) | low;
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


        byte optCode = memory.read(PC);

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
        addr_t nextPC = (addr_t) (PC + 1);
        switch (addressingMode) {
            case ZERO_PAGE:
                address = memory.read(nextPC);
                break;
            case ZERO_PAGE_X:
                address = memory.read(nextPC) + X;
                break;
            case ZERO_PAGE_Y:
                address = memory.read(nextPC) + Y;
                break;
            case ABSOLUTE:
                address = memory.readDoubleByte(nextPC);
                break;
            case ABSOLUTE_X:
                address = memory.readDoubleByte(nextPC) + X;
                pageCrossed = pagesDiffer(address - X, address);
                break;
            case ABSOLUTE_Y:
                address = memory.readDoubleByte(nextPC) + Y;
                pageCrossed = pagesDiffer(address - Y, address);
                break;
            case INDIRECT:
                address = memory.readDoubleByteBugly(memory.readDoubleByte(nextPC));
                break;
            case IMPLIED:
                break;
            case ACCUMULATOR:
                break;
            case IMMEDIATE:
                address = nextPC;
                break;
            case RELATIVE:
                dbyte offset = memory.readDoubleByte(nextPC);
                address = (addr_t) (PC + 2 + offset + (offset < 0x80 ? 0 : -0x100));
                break;
            case INDEXED_INDIRECT:
                address = memory.readDoubleByteBugly(memory.readDoubleByte(nextPC) + X);
                break;
            case INDIRECT_INDEXED:
                address = memory.readDoubleByteBugly(memory.readDoubleByte(nextPC)) + Y;
                pageCrossed = pagesDiffer(address - Y, address);
                break;
            default:
                LOG("Error AddressingMode %d", addressingMode);
        }


        PC += instructionLength;
        this->cycles += cycle;

        if (pageCrossed) {
            this->cycles = pageCycle;
        }

        if (pOperation != nullptr) {
            const Context context = {address, PC, addressingMode};
            this->(*pOperation)(context);
        }

        return this->cycles - cycle;
    }

    // ADC - Add with Carry
    void Cpu::ADC(const Context &context) {
        auto acc = ACC;
        auto addition = memory.read(context.address);
        auto carry = CF;

        uint16_t sum;
        if (DF && supportBCD) {
            sum = bcd(acc) + bcd(addition) + carry;
            CF = sum > 99;
        } else {
            sum = acc + addition + carry;
            CF = sum > 0xff;
        }

        ACC = (byte) sum;
        setZN(ACC);

        VF = !((acc ^ addition) & 0x80) && ((acc ^ ACC) & 0x80);
    }

    // AND - Bitwise-AND A with Memory
    void Cpu::AND(const Context &context) {
        ACC = ACC & memory.read(context.address);
        setZN(ACC);
    }

    /* ASL - Arithmetic Shift Left
    "In my experience, this is NOT an "arithmetic" shift.
     An Arithmetic shift normally preserves the Most Significant Bit (MSb) or "Sign bit" of the source value.
     ASL does NOT do this on the 6502."
    The 6502 places a copy of the sign from the result of a Logical Shift Left into the sigN Flag (P.N)
    This instruction would be better named as SLS (logical Shift Left and update Sign)"
     ---- http://homepage.ntlworld.com/cyborgsystems/CS_Main/6502/6502.htm#xxx*/
    void Cpu::ASL(const Context &context) {
        if (context.mode == ACCUMULATOR) {
            CF = ACC >> 7;
            ACC <<= 1;
            setZN(ACC);
        } else {
            auto value = memory.read(context.address);
            CF = value >> 7;
            value <<= 1;
            memory.write(context.address, value);
            setZN(value);
        }
    }

    // BCC - Branch if CarryFlag is CLEAR
    void Cpu::BCC(const Context &context) {
        if (!CF) {
            pcGoto(context);
        }
    }


    // BCS - Branch if CarryFlag Set
    void Cpu::BCS(const Context &context) {
        if (CF) {
            pcGoto(context);
        }
    }

    // BEQ - Branch if Equal
    void Cpu::BEQ(const Context &context) {
        if (ZF) {
            pcGoto(context);
        }
    }

    // BIT - Bit Test
    void Cpu::BIT(const Context &context) {
        auto value = memory.read(context.address);
        VF = (bit) ((value >> 6) & 1);
        NF = value >> 7;
        ZF = (value & ACC) == 0;
    }

    // BMI - Branch if Minus
    void Cpu::BMI(const Context &context) {
        if (NF) {
            pcGoto(context);
        }
    }

    // BNE - Branch if Not Equal
    void Cpu::BNE(const Context &context) {
        if (!ZF) {
            pcGoto(context);
        }
    }

    // BPL - Branch if Positive
    void Cpu::BPL(const Context &context) {
        if (!NF) {
            pcGoto(context);
        }
    }

    // BRK - Force Interrupt
    void Cpu::BRK(const Context &context) {
        PC++; //may be?
        pushDoubleByte(PC);
        push(getProcessorStatus() | 0x10); // BF = 1
        PC = memory.readDoubleByte(0xFFFE);
    }

    // BVC - Branch if Overflow Clear
    void Cpu::BVC(const Context &context) {
        if (!VF) {
            pcGoto(context);
        }
    }

    // BVS - Branch if Overflow Set
    void Cpu::BVS(const Context &context) {
        if (VF) {
            pcGoto(context);
        }
    }

    // CLC - Clear Carry Flag
    void Cpu::CLC(const Context &context) {
        CF = 0;
    }

    // CLD - Clear Decimal Flag
    void Cpu::CLD(const Context &context) {
        DF = 0;
    }

    // CLI - Clear Interrupt Disable
    void Cpu::CLI(const Context &context) {
        IF = 0;
    }

    // CLV - Clear Overflow Flag
    void Cpu::CLV(const Context &context) {
        VF = 0;
    }

    void Cpu::CMP(const Context &context) {
        auto value = memory.read(context.address);
        compare(ACC, value);
    }

    // CPX - Compare X Register
    void Cpu::CPX(const Context &context) {
        auto value = memory.read(context.address);
        compare(X, value);
    }

    // CPY - Compare Y Register
    void Cpu::CPY(const Context &context) {
        auto value = memory.read(context.address);
        compare(Y, value);
    }

    // DEC - Decrement Memory
    void Cpu::DEC(const Context &context) {
        byte value = memory.read(context.address) - 1;
        memory.write(context.address, value);
        setZN(value);
    }

    // DEX - Decrement X Register
    void Cpu::DEX(const Context &context) {
        --X;
        setZN(X);
    }

    // DEY - Decrement Y Register
    void Cpu::DEY(const Context &context) {
        --Y;
        setZN(Y);
    }

    // EOR - Exclusive OR
    void Cpu::EOR(const Context &context) {
        ACC = ACC ^ memory.read(context.address);
        setZN(ACC);
    }

    // INC - Increment Memory
    void Cpu::INC(const Context &context) {
        byte value = memory.read(context.address) + 1;
        memory.write(context.address, value);
        setZN(value);
    }

    // INX - Increment X Register
    void Cpu::INX(const Context &context) {
        ++X;
        setZN(X);
    }

    // INY - Increment Y Register
    void Cpu::INY(const Context &context) {
        ++Y;
        setZN(Y);
    }

    // JMP - Jump
    void Cpu::JMP(const Context &context) {
        PC = context.address;
    }

    // JSR - Jump to Subroutine
    void Cpu::JSR(const Context &context) {
        pushDoubleByte(PC - 1);
        PC = context.address;
    }

    // LDA - Load Accumulator
    void Cpu::LDA(const Context &context) {
        ACC = memory.read(context.address);
        setZN(ACC);
    }

    // LDX - Load X Register
    void Cpu::LDX(const Context &context) {
        X = memory.read(context.address);
        setZN(X);
    }

    // LDY - Load Y Register
    void Cpu::LDY(const Context &context) {
        Y = memory.read(context.address);
        setZN(Y);
    }

    // LSR - Logical Shift Right
    void Cpu::LSR(const Context &context) {
        if (context.mode == ACCUMULATOR) {
            CF = (bit) (ACC & 1);
            ACC >>= 1;
            setZN(ACC);
        } else {
            auto value = memory.read(context.address);
            CF = (bit) (value & 1);
            memory.write(context.address, value);
            setZN(value);
        }
    }

    // NOP - No Operation
    void Cpu::NOP(const Context &context) {

    }

    // ORA - Logical Inclusive OR
    void Cpu::ORA(const Context &context) {
        ACC |= memory.read(context.address);
        setZN(ACC);
    }

    // PHA - Push Accumulator
    void Cpu::PHA(const Context &context) {
        push(ACC);
    }

    // PHP - Push Processor Status
    void Cpu::PHP(const Context &context) {
        push(getProcessorStatus());
    }

    // PLA - Pull Accumulator
    void Cpu::PLA(const Context &context) {
        ACC = pull();
        setZN(ACC);
    }

    // PLP - Pull Processor Status
    void Cpu::PLP(const Context &context) {
        //TODO a issue??
        setProcessorStatus(pull());
    }

    // ROL - Rotate Left
    void Cpu::ROL(const Context &context) {
        if (context.mode == ACCUMULATOR) {
            auto c = CF;
            CF = (bit) ((ACC >> 7) & 1);
            ACC = (ACC << 1) | c;
            setZN(ACC);
        } else {
            auto c = CF;
            auto value = memory.read(context.address);
            CF = (bit) ((value >> 7) & 1);
            value = (value << 1) | c;
            memory.write(context.address, value);
            setZN(value);
        }
    }

    // ROR - Rotate Right
    void Cpu::ROR(const Context &context) {
        if (context.mode == ACCUMULATOR) {
            auto c = CF;
            CF = (bit) (ACC & 1);
            ACC = (ACC >> 1) | (c << 7);
            setZN(ACC);
        } else {
            auto c = CF;
            auto value = memory.read(context.address);
            CF = (bit) (value & 1);
            value = (value >> 1) | (c << 7);
            memory.write(context.address, value);
            setZN(value);
        }
    }

    // RTI - Return from Interrupt
    void Cpu::RTI(const Context &context) {
        setProcessorStatus(pull());
        PC = pullDoubleByte();
    }

    // RTS - Return from Subroutine
    void Cpu::RTS(const Context &context) {
        PC = (addr_t) (pullDoubleByte() + 1);
    }

    // SBC - Subtract with Carry
    void Cpu::SBC(const Context &context) {
        auto acc = ACC;
        auto subtraction = memory.read(context.address);
        auto carry = CF;

        uint16_t diff;
        if (DF && supportBCD) {
            diff = bcd(acc) - bcd(subtraction) - !carry;
            VF = diff > 99 || diff < 0;
        } else {
            diff = acc - subtraction - !carry;
            VF = diff > 0xff || diff < -0x100;
        }

        CF = diff >= 0;
        ACC = (byte) diff;
        setZN(ACC);
    }

    // SEC - Set Carry Flag
    void Cpu::SEC(const Context &context) {
        CF = 1;
    }

    // SED - Set Decimal Flag
    void Cpu::SED(const Context &context) {
        DF = 1;
    }

    // SEI - Set Interrupt Disable
    void Cpu::SEI(const Context &context) {
        IF = 1;
    }

    // STA - Store Accumulator
    void Cpu::STA(const Context &context) {
        memory.write(context.address,ACC);
    }

    // STX - Store X Register
    void Cpu::STX(const Context &context) {
        memory.write(context.address,X);

    }

    // STY - Store Y Register
    void Cpu::STY(const Context &context) {
        memory.write(context.address,Y);
    }

    // TAX - Transfer Accumulator to X
    void Cpu::TAX(const Context &context) {
        X = ACC;
        setZN(X);
    }

    // TAY - Transfer Accumulator to Y
    void Cpu::TAY(const Context &context) {
        Y = ACC;
        setZN(Y);
    }

    // TSX - Transfer Stack Pointer to X
    void Cpu::TSX(const Context &context) {
        X = SP;
        setZN(X);
    }

    // TXA - Transfer X to Accumulator
    void Cpu::TXA(const Context &context) {
        ACC = X;
        setZN(ACC);
    }

    // TXS - Transfer X to Stack Pointer
    void Cpu::TXS(const Context &context) {
        SP = X;
    }

    // TYA - Transfer Y to Accumulator
    void Cpu::TYA(const Context &context) {
        ACC = Y;
        setZN(ACC);
    }
}

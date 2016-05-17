//
// Created by Cauchywei on 16/5/7.
//

#ifndef NESDROID_CPU_H
#define NESDROID_CPU_H

#include "commons.h"
#include "memory.h"



namespace nesdroid {


    static const byte INIT_SP = 0xFF;
    static const addr_t STACK_BASE = 0x100;
    typedef void (Cpu::*opt)(const Context &context);

    static inline byte bcd(byte value){
        return (byte) ((value >> 4) * 10 + (value & 0xf));
    }


    enum AddressingMode {
        ZERO_PAGE = 0,
        ZERO_PAGE_X = 1,
        ZERO_PAGE_Y,
        ABSOLUTE,
        ABSOLUTE_X,
        ABSOLUTE_Y,
        INDIRECT,
        IMPLIED,
        ACCUMULATOR,
        IMMEDIATE,
        RELATIVE,
        INDEXED_INDIRECT,
        INDIRECT_INDEXED,

    };

    enum Interrupt {
        NONE  = -1,
        MASKABLE_INTERUPT = 0,
        NON_MASKABLE_INTERUPT = 1,
        RESET = 2
    };


    struct Context{
        addr_t address;
        addr_t PC;
        AddressingMode mode;
    };


    class Cpu {

    private:

        bool supportBCD = false;

        CpuMemory memory;

        uint64_t cycles;
        Interrupt interrupt = NONE;
        uint64_t stallCycle;

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


        Cpu() {
        }


        uint64_t excuse();


        void push(byte value);

        void pushDoubleByte(dbyte value);

        byte pull();

        dbyte pullDoubleByte();

        byte getProcessorStatus();

        void setProcessorStatus(byte flags);

        void onResetInterrupt();
        void onMaskableInterrupt();
        void onNonMaskableInterrupt();


        void setZN(const byte &value);
        void compare(byte a, byte b);

//////////////////Instructions////////////////////////////
        //ADC	add with carry
        void ADC(const Context&);

        //AND	and (with accumulator)
        void AND(const Context&);

        //ASL	arithmetic shift left
        void ASL(const Context&);

        //BCC	branch on carry clear
        void BCC(const Context&);

        //BCS	branch on carry set
        void BCS(const Context&);

        //BEQ	branch on equal (zero set)
        void BEQ(const Context&);

        //BIT	bit test
        void BIT(const Context&);

        //BMI	branch on minus (negative set)
        void BMI(const Context&);

        //BNE	branch on not equal (zero clear)
        void BNE(const Context&);

        //BPL	branch on plus (negative clear)
        void BPL(const Context&);

        //BRK	interrupt
        void BRK(const Context&);

        //BVC	branch on overflow clear
        void BVC(const Context&);

        //BVS	branch on overflow set
        void BVS(const Context&);

        //CLC	clear carry
        void CLC(const Context&);

        //CLD	clear decimal
        void CLD(const Context&);

        //CLI	clear interrupt disable
        void CLI(const Context&);

        //CLV	clear overflow
        void CLV(const Context&);

        //CMP	compare (with accumulator)
        void CMP(const Context&);

        //CPX	compare with X
        void CPX(const Context&);

        //CPY	compare with Y
        void CPY(const Context&);

        //DEC	decrement
        void DEC(const Context&);

        //DEX	decrement X
        void DEX(const Context&);

        //DEY	decrement Y
        void DEY(const Context&);

        //EOR	exclusive or (with accumulator)
        void EOR(const Context&);

        //INC	increment
        void INC(const Context&);

        //INX	increment X
        void INX(const Context&);

        //INY	increment Y
        void INY(const Context&);

        //JMP	jump
        void JMP(const Context&);

        //JSR	jump subroutine
        void JSR(const Context&);

        //LDA	load accumulator
        void LDA(const Context&);

        //LDX	load X
        void LDX(const Context&);

        //LDY	load Y
        void LDY(const Context&);

        //LSR	logical shift right
        void LSR(const Context&);

        //NOP	no operation
        void NOP(const Context&);

        //ORA	or with accumulator
        void ORA(const Context&);

        //PHA	push accumulator
        void PHA(const Context&);

        //PHP	push processor status (SR)
        void PHP(const Context&);

        //PLA	pull accumulator
        void PLA(const Context&);

        //PLP	pull processor status (SR)
        void PLP(const Context&);

        //ROL	rotate left
        void ROL(const Context&);

        //ROR	rotate right
        void ROR(const Context&);

        //RTI	return from interrupt
        void RTI(const Context&);

        //RTS	return from subroutine
        void RTS(const Context&);

        //SBC	subtract with carry
        void SBC(const Context&);

        //SEC	set carry
        void SEC(const Context&);

        //SED	set decimal
        void SED(const Context&);

        //SEI	set interrupt disable
        void SEI(const Context&);

        //STA	store accumulator
        void STA(const Context&);

        //STX	store X
        void STX(const Context&);

        //STY	store Y
        void STY(const Context&);

        //TAX	transfer accumulator to X
        void TAX(const Context&);

        //TAY	transfer accumulator to Y
        void TAY(const Context&);

        //TSX	transfer stack pointer to X
        void TSX(const Context&);

        //TXA	transfer X to accumulator
        void TXA(const Context&);

        //TXS	transfer X to stack pointer
        void TXS(const Context&);

        //TYA	transfer Y to accumulator
        void TYA(const Context&);


        void pcGoto(const Context &context);
    };


    static const char *const InstructionNameTable[256] = {
            "BRK", "ORA", "KIL", "SLO", "NOP", "ORA", "ASL", "SLO",
            "PHP", "ORA", "ASL", "ANC", "NOP", "ORA", "ASL", "SLO",
            "BPL", "ORA", "KIL", "SLO", "NOP", "ORA", "ASL", "SLO",
            "CLC", "ORA", "NOP", "SLO", "NOP", "ORA", "ASL", "SLO",
            "JSR", "AND", "KIL", "RLA", "BIT", "AND", "ROL", "RLA",
            "PLP", "AND", "ROL", "ANC", "BIT", "AND", "ROL", "RLA",
            "BMI", "AND", "KIL", "RLA", "NOP", "AND", "ROL", "RLA",
            "SEC", "AND", "NOP", "RLA", "NOP", "AND", "ROL", "RLA",
            "RTI", "EOR", "KIL", "SRE", "NOP", "EOR", "LSR", "SRE",
            "PHA", "EOR", "LSR", "ALR", "JMP", "EOR", "LSR", "SRE",
            "BVC", "EOR", "KIL", "SRE", "NOP", "EOR", "LSR", "SRE",
            "CLI", "EOR", "NOP", "SRE", "NOP", "EOR", "LSR", "SRE",
            "RTS", "ADC", "KIL", "RRA", "NOP", "ADC", "ROR", "RRA",
            "PLA", "ADC", "ROR", "ARR", "JMP", "ADC", "ROR", "RRA",
            "BVS", "ADC", "KIL", "RRA", "NOP", "ADC", "ROR", "RRA",
            "SEI", "ADC", "NOP", "RRA", "NOP", "ADC", "ROR", "RRA",
            "NOP", "STA", "NOP", "SAX", "STY", "STA", "STX", "SAX",
            "DEY", "NOP", "TXA", "XAA", "STY", "STA", "STX", "SAX",
            "BCC", "STA", "KIL", "AHX", "STY", "STA", "STX", "SAX",
            "TYA", "STA", "TXS", "TAS", "SHY", "STA", "SHX", "AHX",
            "LDY", "LDA", "LDX", "LAX", "LDY", "LDA", "LDX", "LAX",
            "TAY", "LDA", "TAX", "LAX", "LDY", "LDA", "LDX", "LAX",
            "BCS", "LDA", "KIL", "LAX", "LDY", "LDA", "LDX", "LAX",
            "CLV", "LDA", "TSX", "LAS", "LDY", "LDA", "LDX", "LAX",
            "CPY", "CMP", "NOP", "DCP", "CPY", "CMP", "DEC", "DCP",
            "INY", "CMP", "DEX", "AXS", "CPY", "CMP", "DEC", "DCP",
            "BNE", "CMP", "KIL", "DCP", "NOP", "CMP", "DEC", "DCP",
            "CLD", "CMP", "NOP", "DCP", "NOP", "CMP", "DEC", "DCP",
            "CPX", "SBC", "NOP", "ISC", "CPX", "SBC", "INC", "ISC",
            "INX", "SBC", "NOP", "SBC", "CPX", "SBC", "INC", "ISC",
            "BEQ", "SBC", "KIL", "ISC", "NOP", "SBC", "INC", "ISC",
            "SED", "SBC", "NOP", "ISC", "NOP", "SBC", "INC", "ISC",
    };

//Mapping instruction opt code to CPU member function
    static const opt InstructionTable[256] = {
            &Cpu::BRK, &Cpu::ORA, nullptr, nullptr, &Cpu::NOP, &Cpu::ORA, &Cpu::ASL, nullptr,
            &Cpu::PHP, &Cpu::ORA, &Cpu::ASL, nullptr, &Cpu::NOP, &Cpu::ORA, &Cpu::ASL, nullptr,
            &Cpu::BPL, &Cpu::ORA, nullptr, nullptr, &Cpu::NOP, &Cpu::ORA, &Cpu::ASL, nullptr,
            &Cpu::CLC, &Cpu::ORA, &Cpu::NOP, nullptr, &Cpu::NOP, &Cpu::ORA, &Cpu::ASL, nullptr,
            &Cpu::JSR, &Cpu::AND, nullptr, nullptr, &Cpu::BIT, &Cpu::AND, &Cpu::ROL, nullptr,
            &Cpu::PLP, &Cpu::AND, &Cpu::ROL, nullptr, &Cpu::BIT, &Cpu::AND, &Cpu::ROL, nullptr,
            &Cpu::BMI, &Cpu::AND, nullptr, nullptr, &Cpu::NOP, &Cpu::AND, &Cpu::ROL, nullptr,
            &Cpu::SEC, &Cpu::AND, &Cpu::NOP, nullptr, &Cpu::NOP, &Cpu::AND, &Cpu::ROL, nullptr,
            &Cpu::RTI, &Cpu::EOR, nullptr, nullptr, &Cpu::NOP, &Cpu::EOR, &Cpu::LSR, nullptr,
            &Cpu::PHA, &Cpu::EOR, &Cpu::LSR, nullptr, &Cpu::JMP, &Cpu::EOR, &Cpu::LSR, nullptr,
            &Cpu::BVC, &Cpu::EOR, nullptr, nullptr, &Cpu::NOP, &Cpu::EOR, &Cpu::LSR, nullptr,
            &Cpu::CLI, &Cpu::EOR, &Cpu::NOP, nullptr, &Cpu::NOP, &Cpu::EOR, &Cpu::LSR, nullptr,
            &Cpu::RTS, &Cpu::ADC, nullptr, nullptr, &Cpu::NOP, &Cpu::ADC, &Cpu::ROR, nullptr,
            &Cpu::PLA, &Cpu::ADC, &Cpu::ROR, nullptr, &Cpu::JMP, &Cpu::ADC, &Cpu::ROR, nullptr,
            &Cpu::BVS, &Cpu::ADC, nullptr, nullptr, &Cpu::NOP, &Cpu::ADC, &Cpu::ROR, nullptr,
            &Cpu::SEI, &Cpu::ADC, &Cpu::NOP, nullptr, &Cpu::NOP, &Cpu::ADC, &Cpu::ROR, nullptr,
            &Cpu::NOP, &Cpu::STA, &Cpu::NOP, nullptr, &Cpu::STY, &Cpu::STA, &Cpu::STX, nullptr,
            &Cpu::DEY, &Cpu::NOP, &Cpu::TXA, nullptr, &Cpu::STY, &Cpu::STA, &Cpu::STX, nullptr,
            &Cpu::BCC, &Cpu::STA, nullptr, nullptr, &Cpu::STY, &Cpu::STA, &Cpu::STX, nullptr,
            &Cpu::TYA, &Cpu::STA, &Cpu::TXS, nullptr, nullptr, &Cpu::STA, nullptr, nullptr,
            &Cpu::LDY, &Cpu::LDA, &Cpu::LDX, nullptr, &Cpu::LDY, &Cpu::LDA, &Cpu::LDX, nullptr,
            &Cpu::TAY, &Cpu::LDA, &Cpu::TAX, nullptr, &Cpu::LDY, &Cpu::LDA, &Cpu::LDX, nullptr,
            &Cpu::BCS, &Cpu::LDA, nullptr, nullptr, &Cpu::LDY, &Cpu::LDA, &Cpu::LDX, nullptr,
            &Cpu::CLV, &Cpu::LDA, &Cpu::TSX, nullptr, &Cpu::LDY, &Cpu::LDA, &Cpu::LDX, nullptr,
            &Cpu::CPY, &Cpu::CMP, &Cpu::NOP, nullptr, &Cpu::CPY, &Cpu::CMP, &Cpu::DEC, nullptr,
            &Cpu::INY, &Cpu::CMP, &Cpu::DEX, nullptr, &Cpu::CPY, &Cpu::CMP, &Cpu::DEC, nullptr,
            &Cpu::BNE, &Cpu::CMP, nullptr, nullptr, &Cpu::NOP, &Cpu::CMP, &Cpu::DEC, nullptr,
            &Cpu::CLD, &Cpu::CMP, &Cpu::NOP, nullptr, &Cpu::NOP, &Cpu::CMP, &Cpu::DEC, nullptr,
            &Cpu::CPX, &Cpu::SBC, &Cpu::NOP, nullptr, &Cpu::CPX, &Cpu::SBC, &Cpu::INC, nullptr,
            &Cpu::INX, &Cpu::SBC, &Cpu::NOP, &Cpu::SBC, &Cpu::CPX, &Cpu::SBC, &Cpu::INC, nullptr,
            &Cpu::BEQ, &Cpu::SBC, nullptr, nullptr, &Cpu::NOP, &Cpu::SBC, &Cpu::INC, nullptr,
            &Cpu::SED, &Cpu::SBC, &Cpu::NOP, nullptr, &Cpu::NOP, &Cpu::SBC, &Cpu::INC, nullptr,
    };


//Addressing mode of instructions
    static const AddressingMode AddressingModeTable[256] = {
            IMPLIED, INDEXED_INDIRECT, IMPLIED, INDEXED_INDIRECT, ZERO_PAGE, ZERO_PAGE, ZERO_PAGE, ZERO_PAGE,
            IMPLIED, IMMEDIATE, ACCUMULATOR, IMMEDIATE, ABSOLUTE, ABSOLUTE, ABSOLUTE, ABSOLUTE,
            RELATIVE, INDIRECT_INDEXED, IMPLIED, INDIRECT_INDEXED, ZERO_PAGE_X, ZERO_PAGE_X, ZERO_PAGE_X, ZERO_PAGE_X,
            IMPLIED, ABSOLUTE_Y, IMPLIED, ABSOLUTE_Y, ABSOLUTE_X, ABSOLUTE_X, ABSOLUTE_X, ABSOLUTE_X,
            ABSOLUTE, INDEXED_INDIRECT, IMPLIED, INDEXED_INDIRECT, ZERO_PAGE, ZERO_PAGE, ZERO_PAGE, ZERO_PAGE,
            IMPLIED, IMMEDIATE, ACCUMULATOR, IMMEDIATE, ABSOLUTE, ABSOLUTE, ABSOLUTE, ABSOLUTE,
            RELATIVE, INDIRECT_INDEXED, IMPLIED, INDIRECT_INDEXED, ZERO_PAGE_X, ZERO_PAGE_X, ZERO_PAGE_X, ZERO_PAGE_X,
            IMPLIED, ABSOLUTE_Y, IMPLIED, ABSOLUTE_Y, ABSOLUTE_X, ABSOLUTE_X, ABSOLUTE_X, ABSOLUTE_X,
            IMPLIED, INDEXED_INDIRECT, IMPLIED, INDEXED_INDIRECT, ZERO_PAGE, ZERO_PAGE, ZERO_PAGE, ZERO_PAGE,
            IMPLIED, IMMEDIATE, ACCUMULATOR, IMMEDIATE, ABSOLUTE, ABSOLUTE, ABSOLUTE, ABSOLUTE,
            RELATIVE, INDIRECT_INDEXED, IMPLIED, INDIRECT_INDEXED, ZERO_PAGE_X, ZERO_PAGE_X, ZERO_PAGE_X, ZERO_PAGE_X,
            IMPLIED, ABSOLUTE_Y, IMPLIED, ABSOLUTE_Y, ABSOLUTE_X, ABSOLUTE_X, ABSOLUTE_X, ABSOLUTE_X,
            IMPLIED, INDEXED_INDIRECT, IMPLIED, INDEXED_INDIRECT, ZERO_PAGE, ZERO_PAGE, ZERO_PAGE, ZERO_PAGE,
            IMPLIED, IMMEDIATE, ACCUMULATOR, IMMEDIATE, INDIRECT, ABSOLUTE, ABSOLUTE, ABSOLUTE,
            RELATIVE, INDIRECT_INDEXED, IMPLIED, INDIRECT_INDEXED, ZERO_PAGE_X, ZERO_PAGE_X, ZERO_PAGE_X, ZERO_PAGE_X,
            IMPLIED, ABSOLUTE_Y, IMPLIED, ABSOLUTE_Y, ABSOLUTE_X, ABSOLUTE_X, ABSOLUTE_X, ABSOLUTE_X,
            IMMEDIATE, INDEXED_INDIRECT, IMMEDIATE, INDEXED_INDIRECT, ZERO_PAGE, ZERO_PAGE, ZERO_PAGE, ZERO_PAGE,
            IMPLIED, IMMEDIATE, IMPLIED, IMMEDIATE, ABSOLUTE, ABSOLUTE, ABSOLUTE, ABSOLUTE,
            RELATIVE, INDIRECT_INDEXED, IMPLIED, INDIRECT_INDEXED, ZERO_PAGE_X, ZERO_PAGE_X, ZERO_PAGE_Y, ZERO_PAGE_Y,
            IMPLIED, ABSOLUTE_Y, IMPLIED, ABSOLUTE_Y, ABSOLUTE_X, ABSOLUTE_X, ABSOLUTE_Y, ABSOLUTE_Y,
            IMMEDIATE, INDEXED_INDIRECT, IMMEDIATE, INDEXED_INDIRECT, ZERO_PAGE, ZERO_PAGE, ZERO_PAGE, ZERO_PAGE,
            IMPLIED, IMMEDIATE, IMPLIED, IMMEDIATE, ABSOLUTE, ABSOLUTE, ABSOLUTE, ABSOLUTE,
            RELATIVE, INDIRECT_INDEXED, IMPLIED, INDIRECT_INDEXED, ZERO_PAGE_X, ZERO_PAGE_X, ZERO_PAGE_Y, ZERO_PAGE_Y,
            IMPLIED, ABSOLUTE_Y, IMPLIED, ABSOLUTE_Y, ABSOLUTE_X, ABSOLUTE_X, ABSOLUTE_Y, ABSOLUTE_Y,
            IMMEDIATE, INDEXED_INDIRECT, IMMEDIATE, INDEXED_INDIRECT, ZERO_PAGE, ZERO_PAGE, ZERO_PAGE, ZERO_PAGE,
            IMPLIED, IMMEDIATE, IMPLIED, IMMEDIATE, ABSOLUTE, ABSOLUTE, ABSOLUTE, ABSOLUTE,
            RELATIVE, INDIRECT_INDEXED, IMPLIED, INDIRECT_INDEXED, ZERO_PAGE_X, ZERO_PAGE_X, ZERO_PAGE_X, ZERO_PAGE_X,
            IMPLIED, ABSOLUTE_Y, IMPLIED, ABSOLUTE_Y, ABSOLUTE_X, ABSOLUTE_X, ABSOLUTE_X, ABSOLUTE_X,
            IMMEDIATE, INDEXED_INDIRECT, IMMEDIATE, INDEXED_INDIRECT, ZERO_PAGE, ZERO_PAGE, ZERO_PAGE, ZERO_PAGE,
            IMPLIED, IMMEDIATE, IMPLIED, IMMEDIATE, ABSOLUTE, ABSOLUTE, ABSOLUTE, ABSOLUTE,
            RELATIVE, INDIRECT_INDEXED, IMPLIED, INDIRECT_INDEXED, ZERO_PAGE_X, ZERO_PAGE_X, ZERO_PAGE_X, ZERO_PAGE_X,
            IMPLIED, ABSOLUTE_Y, IMPLIED, ABSOLUTE_Y, ABSOLUTE_X, ABSOLUTE_X, ABSOLUTE_X, ABSOLUTE_X,
    };

// The number of bytes of memory required to store the instruction.
    static const byte InstructionLengthTable[256] = {
            1, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
            2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
            3, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
            2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
            1, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
            2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
            1, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
            2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
            2, 2, 0, 0, 2, 2, 2, 0, 1, 0, 1, 0, 3, 3, 3, 0,
            2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 0, 3, 0, 0,
            2, 2, 2, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
            2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
            2, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
            2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
            2, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
            2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
    };

// The number of clock cycles required to execute the instruction.
    static const byte InstructionCycleTable[256] = {
            7, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6,
            2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
            6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 4, 4, 6, 6,
            2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
            6, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 3, 4, 6, 6,
            2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
            6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 5, 4, 6, 6,
            2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
            2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
            2, 6, 2, 6, 4, 4, 4, 4, 2, 5, 2, 5, 5, 5, 5, 5,
            2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
            2, 5, 2, 5, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4,
            2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
            2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
            2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
            2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
    };

// The number of cycles used by each instruction when a page is crossed
    static const byte InstructionPageCycleTable[256] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
    };
}

#endif //NESDROID_CPU_H


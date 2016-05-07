package org.sssta.nesdroid.emulator;

/**
 * Created by cauchywei on 15/11/18.
 */
public class Cpu {

    //Interrupt type
    public static final int IRQ_MI = 0;
    public static final int IRQ_NMI = 1;
    public static final int IRQ_RESET = 2;

    public static final int STACK_OFFSET = 0x0100;

    //Registers
    private int ACC;// 8bit
    private int X;  // 8bit
    private int Y;  // 8bit

    //Stack Pointer, 8bit, it decrease when push
    //Stack at memory locations $0100-$01FF
    private int SP;

    //Program Counter, 16bit, point next instruction address
    private int PC;

    //Processor Status
    // 7 6 5 4 3 2 1 0
    // N V   B D I Z C
    private boolean CF; //Carry Flag
    private boolean ZF; //Zero Flag
    private boolean IF; //Interrupt Disable
    private boolean DF; //Decimal Mode
    private boolean BF; //Break Command
    //              --; //Empty
    private boolean VF; //Overflow Flag
    private boolean NF; //Negative Flag

    public void init() {

    }

    public void reset() {

    }
}

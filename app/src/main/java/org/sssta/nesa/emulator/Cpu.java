package org.sssta.nesa.emulator;

/**
 * Created by cauchywei on 15/11/18.
 */
public class Cpu {

    //registers
    private int ACC;
    private int X;
    private int Y;
    private int SP;
    private int PC;

    //Processor Status
    // 7 6 5 4 3 2 1 0
    // N V   B D I Z C
    private boolean CF; //Carry Flag
    private boolean ZF; //Zero Flag
    private boolean IF; //Interrupt Disable
    private boolean DF; //Decimal Mode
    private boolean BF; //Break Command

    private boolean VF; //Overflow Flag
    private boolean NF; //Negative Flag

    public void init() {

    }

    public void reset() {

    }
}

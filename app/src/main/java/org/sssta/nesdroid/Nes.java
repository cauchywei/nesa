package org.sssta.nesdroid;

/**
 * Created by cauchywei on 16/5/7.
 */
public class Nes {

    static {
        System.loadLibrary("nes-simulator-jni");
    }

    public native void init();
    public native void reset();
    public native void release();

}

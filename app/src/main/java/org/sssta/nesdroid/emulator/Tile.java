package org.sssta.nesdroid.emulator;

/**
 * Created by cauchywei on 16/5/5.
 */
public class Tile {

    public int fbIndex;
    public int tIndex;
    public int x;
    public int y;
    public int width;
    public int height;
    public int incX;
    public int incY;
    public int palIndex;
    public int tpri;
    public int c;

    public byte[] pixels = new byte[64];
    public byte[] opaque = new byte[8];

    public boolean initialized;

    public void setScanline(int leftOver, byte b, byte b1) {

    }
}

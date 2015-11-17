package org.sssta.nesdroid.emulator;

import android.support.annotation.NonNull;

import org.sssta.nesdroid.utils.FileUtils;

import java.io.IOException;

/**
 * Created by cauchywei on 15/11/17.
 */
public class Nes {

    public static final int VERTICAL_MIRROR = 1;
    public static final int HORIZONAL_MIRROR = 0;

    private String path;
    private boolean valid;
    private byte[] content;

    private int romCount;
    private int vromCount;
    private int mirrorType;
    private boolean hasBatteryRam;
    private boolean hasTrainer;
    private boolean hasFourScreen;
    private int romMapperType;

    private int[][] rom;
    private int[][] vrom;

    public Nes(@NonNull String path) throws IOException{
        this.path = path;

        content = FileUtils.getContent(path);
        if (content != null) {
            parse();
        }

    }

    private void parse() {

        if (content.length < 4)
            return;

        //NES file start with "NES\x1a"
        if (!(content[0] == 'N' && content[1] == 'E' && content[2] == 'S' && content[3] == '\u001a')) {
            return;
        }

        romCount = content[4];
        vromCount = content[5];

        mirrorType = (byte) (content[6] & 1) == 0?HORIZONAL_MIRROR:VERTICAL_MIRROR;
        hasBatteryRam = ((content[6] & 0b10) >> 1) == 1;
        hasTrainer = ((content[6] & 0b100) >> 2) == 1;
        hasFourScreen = ((content[6] & 0b1000) >> 3) == 1;
        romMapperType = ((content[6] & 0xF0) >> 4) | (content[7] & 0xF0 );

        if (hasBatteryRam) {
            //TODO
        }


        if (((content[7] & 0xF ) != 0)) {
            return;
        }

        boolean error = false;
        for (int i = 8; i < 0xf; i++) {
            if (content[i] != 0) {
                error = true;
                break;
            }
        }

        if (error) {
            return;
        }

        final int romUnitSize =  16*1024;
        int offset = 0xf;
        rom = new int[romCount][];
        for (int i = 0; i < rom.length; i++) {
            rom[i] = new int[romUnitSize];
            for (int j=0; j< romUnitSize; j++) {
                int index = offset + j;
                if (index >= content.length){
                    break;
                }
                this.vrom[i][j] =content[index];
            }
            offset += romUnitSize;
        }


        final int vromUnitSize =  8*1024;
        vrom = new int[vromCount][];
        for (int i = 0; i < vrom.length; i++) {
            vrom[i] = new int[vromUnitSize];
            for (int j=0; j< vromUnitSize; j++) {
                int index = offset + j;
                if (index >= content.length){
                    break;
                }
                this.vrom[i][j] =content[index];
            }
            offset += vromUnitSize;
        }


        valid = true;

    }

    public String getPath() {
        return path;
    }


    public boolean isValid() {
        return valid;
    }

    public byte[] getContent() {
        return content;
    }

}

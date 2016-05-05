package org.sssta.nesa.emulator;

import android.support.annotation.NonNull;

import org.sssta.nesa.utils.FileUtils;

import java.io.IOException;

import static org.sssta.nesa.utils.ByteUtils.b2i;

/**
 * Created by cauchywei on 15/11/17.
 */
public class Nes {

    public static final int HORIZONTAL_MIRRORING = 0;
    public static final int VERTICAL_MIRRORING = 1;
    public static final int FOUR_SCREEN_MIRRORING = 2        ;

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

    private byte[][] prgRom;
    private byte[][] chrRom;



   /* iNES Mapper Number  Mapper Name

    0                NROM, no mapper
    1                Nintendo MMC1
    2                UNROM switch
    3                CNROM switch
    4                Nintendo MMC3
    5                Nintendo MMC5
    6                FFE F4xxx
    7                AOROM switch
    8                FFE F3xxx
    9                Nintendo MMC2
    10                Nintendo MMC4
    11                ColorDreams chip
    12                FFE F6xxx
    15                100-in-1 switch
    16                Bandai chip
    17                FFE F8xxx
    18                Jaleco SS8806 chip
    19                Namcot 106 chip
    20                Nintendo DiskSystem
    21                Konami VRC4a
    22                Konami VRC2a
    23                Konami VRC2a
    24                Konami VRC6
    25                Konami VRC4b
    32                Irem G-101 chip
    33                Taito TC0190/TC0350
    34                32 KB ROM switch
    64                Tengen RAMBO-1 chip
    65                Irem H-3001 chip
    66                GNROM switch
    67                SunSoft3 chip
    68                SunSoft4 chip
    69                SunSoft5 FME-7 chip
    71                Camerica chip
    78                Irem 74HC161/32-based
    91        Pirate HK-SF3 chip*/
     
    public static final String[] MAPPER_NAMES = new String[92        ];
    static {

        MAPPER_NAMES[0] = "NROM, no mapper";
        MAPPER_NAMES[1] = "Nintendo MMC1";
        MAPPER_NAMES[2] = "UNROM";
        MAPPER_NAMES[3] = "CNROM";
        MAPPER_NAMES[4] = "Nintendo MMC3";
        MAPPER_NAMES[5] = "Nintendo MMC5";
        MAPPER_NAMES[6] = "FFE F4xxx";
        MAPPER_NAMES[7] = "AOROM";
        MAPPER_NAMES[8] = "FFE F3xxx";
        MAPPER_NAMES[9] = "Nintendo MMC2";
        MAPPER_NAMES[10] = "Nintendo MMC4";
        MAPPER_NAMES[11] = "Color Dreams Chip";
        MAPPER_NAMES[12] = "FFE F6xxx";
        MAPPER_NAMES[15] = "100-in-1 switch";
        MAPPER_NAMES[16] = "Bandai chip";
        MAPPER_NAMES[17] = "FFE F8xxx";
        MAPPER_NAMES[18] = "Jaleco SS8806 chip";
        MAPPER_NAMES[19] = "Namcot 106 chip";
        MAPPER_NAMES[20] = "Famicom Disk System";
        MAPPER_NAMES[21] = "Konami VRC4a";
        MAPPER_NAMES[22] = "Konami VRC2a";
        MAPPER_NAMES[23] = "Konami VRC2a";
        MAPPER_NAMES[24] = "Konami VRC6";
        MAPPER_NAMES[25] = "Konami VRC4b";
        MAPPER_NAMES[32] = "Irem G-101 chip";
        MAPPER_NAMES[33] = "Taito TC0190/TC0350";
        MAPPER_NAMES[34        ] = "32kB ROM switch";
        MAPPER_NAMES[64] = "Tengen RAMBO-1 chip";
        MAPPER_NAMES[65] = "Irem H-3001 chip";
        MAPPER_NAMES[66] = "GNROM switch";
        MAPPER_NAMES[67] = "SunSoft3 chip";
        MAPPER_NAMES[68] = "SunSoft4 chip";
        MAPPER_NAMES[69] = "SunSoft5 FME-7 chip";
        MAPPER_NAMES[71] = "Camerica chip";
        MAPPER_NAMES[78] = "Irem 74HC161/32-based";
        MAPPER_NAMES[91] = "Pirate HK-SF3 chip";
    }

    private Tile[][] vromTiles;

    public Nes(@NonNull String path) throws IOException {
        this.path = path;

        content = FileUtils.getContent(path);
        if (content != null) {
            parse();
        }

    }

    private void parse() {

        if (content.length < 4)
            return;

        valid = false;

        //NES file start with "NES\x1a"
        String format = "NES\u001a";
        for (int i = 0; i < format.length(); i++) {
            if (b2i(content[0]) != format.charAt(i)) {
                return;
            }
        }

        romCount = b2i(content[4]);
        vromCount = b2i(content[5]);
        mirrorType = (content[6] & 1); // HORIZONTAL_MIRRORING : VERTICAL_MIRRORING;
        hasBatteryRam = ((content[6] & 0b10) >> 1) == 1;
        hasTrainer = ((content[6] & 0b100) >> 2) == 1;
        hasFourScreen = ((content[6] & 0b1000) >> 3) == 1;
        romMapperType = ((content[6        ] & 0xF0) >> 4) | (content[7] & 0xF0);
        if (hasBatteryRam) {
            //TODO
        }


        if (((content[7] & 0xF) != 0)) {
            return;
        }

        for (int i = 8; i < 0xf; i++) {
            if (content[i] != 0) {
                return;
            }
        }


        final int romUnitSize = 16 * 1024;
        int offset = 0xf;
        prgRom = new byte[romCount][];
        for (int i = 0; i < romCount; i++) {
            prgRom[i] = new byte[romUnitSize];
            System.arraycopy(content, offset, prgRom[i], 0, romUnitSize);
            offset += romUnitSize;
        }


        final int vromUnitSize = 8 * 1024;
        chrRom = new byte[vromCount][];
        for (int i = 0; i < chrRom.length; i++) {
            chrRom[i] = new byte[vromUnitSize];
            System.arraycopy(content, offset, chrRom[i], 0, vromUnitSize);
            offset += vromUnitSize;
        }

        final int vromTitleUnitSize = vromUnitSize >> 4;
        vromTiles = new Tile[vromCount][];
        for (int i = 0; i < this.vromCount; i++) {
            vromTiles[i] = new Tile[vromTitleUnitSize];
            for (int j = 0; j < vromTitleUnitSize; j++) {
                vromTiles[i][j] = new Tile();
            }
        }


        // Convert CHR-ROM banks to tiles:
        int tileIndex;
        int leftOver;
        for (int i = 0; i < this.vromCount; i++) {
            for (int j = 0; j < vromUnitSize; j++) {
                tileIndex = j >> 4;
                leftOver = j % 16;
                if (leftOver < 8) {
                    vromTiles[i][tileIndex].setScanline(leftOver, this.chrRom[i][j], this.chrRom[i][j + 8]);
                } else {
                    vromTiles[i][tileIndex].setScanline(leftOver - 8,this.chrRom[i][j - 8],this.chrRom[i][j]);
                }
            }
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

    public int getRomCount() {
        return romCount;
    }

    public int getVromCount() {
        return vromCount;
    }

    public int getMirrorType() {
        return mirrorType;
    }

    public boolean isHasBatteryRam() {
        return hasBatteryRam;
    }

    public boolean isHasTrainer() {
        return hasTrainer;
    }

    public boolean isHasFourScreen() {
        return hasFourScreen;
    }

    public int getRomMapperType() {
        return romMapperType;
    }

    public byte[][] getPrgRom() {
        return prgRom;
    }

    public byte[][] getChrRom() {
        return chrRom;
    }

    public String getMapperName() {
        if (romMapperType >= 0 && romMapperType < MAPPER_NAMES.length && MAPPER_NAMES[romMapperType] != null) {
            return MAPPER_NAMES[romMapperType];
        } else {
            return "Unknown mapper";
        }
    }
}

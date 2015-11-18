package org.sssta.nesa.utils;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

/**
 * Created by cauchywei on 15/11/17.
 */
public class FileUtils {

    public static byte[] getContent(String filePath) throws IOException {
        File file = new File(filePath);
        long fileSize = file.length();
        if (fileSize > Integer.MAX_VALUE) {
            LogUtils.LOGE("FileRead",filePath + " is too big.");
            return null;
        }
        FileInputStream fi = new FileInputStream(file);
        byte[] buffer = new byte[(int) fileSize];
        int offset = 0;
        int numRead = 0;
        while (offset < buffer.length
                && (numRead = fi.read(buffer, offset, buffer.length - offset)) >= 0) {
            offset += numRead;
        }

        if (offset != buffer.length) {
            throw new IOException("Could not completely read file "
                    + file.getName());
//            LogUtils.LOGE("FileRead","Could not completely read file " + filePath);
//            return null;
        }
        fi.close();
        return buffer;
    }

}

package macs;

import java.util.Arrays;
import javax.crypto.IllegalBlockSizeException;


/**
 *
 * @author ZAK
 */
public class Utils {

    public static String toHex(byte buf[]) {
        StringBuffer strbuf = new StringBuffer(buf.length * 2);
        int i;

        for (i = 0; i < buf.length; i++) {
            if (((int) buf[i] & 0xff) < 0x10) {
                strbuf.append("0");
            }

            strbuf.append(Long.toString((int) buf[i] & 0xff, 16));
        }

        return strbuf.toString();
    }

    /**
     *
     * @param size Size of the buffer to create
     * @return
     */
    public static byte[] createBuffer(int size) {
        byte[] buf = new byte[size];
        int loop;

        for(loop=0; loop < size; loop++) {
            buf[loop] = (byte) (loop % 256);
        }
        return buf;
    }
    
    public static byte[] createBuffer(int size, byte fill) {
        byte[] buf = new byte[size];
        int loop;

        for(loop=0; loop < size; loop++) {
            buf[loop] = fill;
        }
        return buf;
    }

    public static byte[] concat(byte[] a, byte[] b) {
        byte[] result = Arrays.copyOf(a, a.length + b.length);
        System.arraycopy(b, 0, result, a.length, b.length);
        return result;
    }
    
    public static byte[] summod2(byte[] a, byte[] b) {      
        byte[] result = Arrays.copyOf(a, (a.length > b.length ? a.length : b.length));
        for(int i = 0; i < b.length; ++i) {
            result[i] ^= b[i];
        }
        return result;
    }


}

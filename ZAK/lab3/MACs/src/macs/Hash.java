package macs;

import java.io.IOException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;
import java.util.Date;

/* Autor: Marius Rejdak

Wyniki:
1MB: 351ms
10MB: 1092ms

*/

/**
 *
 * @author ZAK
 */
public class Hash {

    public static void main(String[] args) throws NoSuchAlgorithmException, IOException {
        long startTime = new Date().getTime();

        byte[] key = "0123456789ABCDEF".getBytes();
        byte[] a_key_pad = Utils.summod2(key, Utils.createBuffer(64, (byte)0x5c));
        byte[] b_key_pad = Utils.summod2(key, Utils.createBuffer(64, (byte)0x36));
        byte[] buf = new byte[10000000];

        int length = 0;
        int in;
        while((in = System.in.read()) != -1) {
            buf[length++] = (byte)in;
        }

        if(length < 10000000)
            buf = Arrays.copyOf(buf, length);

        MessageDigest md = MessageDigest.getInstance("SHA-256");
        md.update(Utils.concat(b_key_pad, buf));
        byte[] digest = md.digest();

        md.update(Utils.concat(a_key_pad, digest));
        digest = md.digest();

        long endTime = new Date().getTime();
        System.out.println("HMAC SHA-256: " + digest.length + " " + Utils.toHex(digest) );
        System.out.println("Time: " + (endTime-startTime) + "ms");
    }
}

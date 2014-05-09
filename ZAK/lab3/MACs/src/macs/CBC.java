
package macs;

import java.util.Date;
import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;

/* Autor: Marius Rejdak

Wyniki:
1MB: 783ms
10MB: 1534ms

*/

/**
 *
 * @author ZAK
 */
public class CBC {
    public static void main(String[] args) throws Exception
    {
        long startTime = new Date().getTime();

        byte[] h0 = Utils.createBuffer(16);
        byte[] h1 = new byte[16];
        byte[] key = "0123456789ABCDEF".getBytes();
        SecretKeySpec skeySpec = new SecretKeySpec(key, "AES");

        Cipher cipher = Cipher.getInstance("AES/ECB/NoPadding");
        cipher.init(Cipher.ENCRYPT_MODE, skeySpec);

        while(System.in.read(h1, 0, 16) == 16) {
            h0 = cipher.doFinal(Utils.summod2(h0, h1));
        }

        long endTime = new Date().getTime();
        System.out.println("CBC-MAC AES: " + h0.length + " " + Utils.toHex(h0));
        System.out.println("Time: " + (endTime-startTime) + "ms");
    }
}

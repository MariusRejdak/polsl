package keys;

import java.math.BigInteger;
import java.security.*;
import java.util.Date;
import javax.crypto.*;


/**
 *
 * @author ZAK
 */
public class RSA {


    private BigInteger n, d, e;

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
     * @param args the command line arguments
     * @throws Exception
     */
    public static void main(String[] args) throws Exception {

        KeyPairGenerator keyGen = KeyPairGenerator.getInstance("RSA");

        SecureRandom random = new SecureRandom();

        keyGen.initialize(3072, random);

        KeyPair pair = keyGen.generateKeyPair();
        
        int i;
        long startDate = new Date().getTime();
        for(i = 0; i < 500; ++i)
        {
            PrivateKey priv = pair.getPrivate();
            PublicKey pub = pair.getPublic();
            
            Cipher rsaCipher = Cipher.getInstance("RSA");
            rsaCipher.init(Cipher.ENCRYPT_MODE, priv);

            KeyGenerator sKeyGen = KeyGenerator.getInstance("AES");
            sKeyGen.init(128);
            Key symmetricKey = sKeyGen.generateKey();
        
            byte[] clearText = symmetricKey.getEncoded();
            byte[] cipherText = null;

            cipherText = rsaCipher.doFinal(clearText);

            rsaCipher.init(Cipher.DECRYPT_MODE, pub);

            byte[] recText = rsaCipher.doFinal(cipherText);
        
        }
        long endDate = new Date().getTime();
        
        System.out.println("Czas "+ i +" wymian klucza: " + (endDate - startDate));
    }
}

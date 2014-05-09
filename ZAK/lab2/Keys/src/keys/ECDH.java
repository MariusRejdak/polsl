/* FIPS curves over prime fields:
 * P-192 secp192r1
 * P-224 secp224r1
 * P-256 secp256r1
 * P-384 secp384r1
 * P-521 secp521r1
 */

package keys;


import java.math.BigInteger;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.MessageDigest;
import java.security.SecureRandom;

import java.security.spec.ECFieldFp;
import java.security.spec.ECGenParameterSpec;
import java.security.spec.ECParameterSpec;
import java.security.spec.ECPoint;
import java.security.spec.EllipticCurve;
import java.util.Date;
import javax.crypto.KeyAgreement;
import javax.crypto.spec.DHParameterSpec;

/**
 *
 * @author ZAK
 */

public class ECDH {

    private final static SecureRandom random = new SecureRandom();

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

    public static void main(String[] args) throws Exception {
        KeyPairGenerator keyGen = KeyPairGenerator.getInstance("EC");
        ECGenParameterSpec ecSpec = new ECGenParameterSpec("secp256r1");

        keyGen.initialize(ecSpec, random);
        
        int i;
        long startDate = new Date().getTime();
        for(i = 0; i < 500; ++i)
        {
            // Alice
            KeyAgreement aKeyAgree = KeyAgreement.getInstance("ECDH");
            KeyPair aPair = keyGen.generateKeyPair();
            aKeyAgree.init(aPair.getPrivate());

            // Bob
            KeyAgreement bKeyAgree = KeyAgreement.getInstance("ECDH");
            KeyPair bPair = keyGen.generateKeyPair();
            bKeyAgree.init(bPair.getPrivate());

            // Alice and Bob public key exchange

            // Alice with Bob's public key
            aKeyAgree.doPhase(bPair.getPublic(), true);

            // Bob with Alice's public key
            bKeyAgree.doPhase(aPair.getPublic(), true);

            byte[] aSymKey = aKeyAgree.generateSecret();
            byte[] bSymKey = bKeyAgree.generateSecret();
        }
        long endDate = new Date().getTime();
        
        System.out.println("Czas "+ i +" wymian klucza: " + (endDate - startDate));
    }
}

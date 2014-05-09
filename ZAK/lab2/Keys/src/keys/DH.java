/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package keys;

import java.math.BigInteger;
import java.security.AlgorithmParameterGenerator;
import java.security.AlgorithmParameters;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.MessageDigest;

import java.security.SecureRandom;
import java.security.spec.AlgorithmParameterSpec;
import java.util.Date;
import javax.crypto.KeyAgreement;
import javax.crypto.spec.DHParameterSpec;

/**
 * @author ZAK
 */

public class DH {

    private static BigInteger p = new BigInteger(
             "d3a6b0bf01f9546245aabfb5bae0bfb890144a6a3c74591eecfc825a"
            +"e464456cd6dbfbb799e78e7cb086169ba789f973524f9a00491779d5"
            +"c10a112289951c5f", 16);
    private static BigInteger g = new BigInteger("7",10);

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

        AlgorithmParameterGenerator apg = AlgorithmParameterGenerator.getInstance("DH");
        apg.init(1024);
        AlgorithmParameters params = apg.generateParameters();
        AlgorithmParameterSpec dhParams = params.getParameterSpec(DHParameterSpec.class);

        KeyPairGenerator keyGen = KeyPairGenerator.getInstance("DH");

        keyGen.initialize(dhParams, random);
        
        int i;
        long startDate = new Date().getTime();
        for(i = 0; i < 500; ++i)
        {
            // Alice
            KeyAgreement aKeyAgree = KeyAgreement.getInstance("DH");
            KeyPair aPair = keyGen.generateKeyPair();
            aKeyAgree.init(aPair.getPrivate());

            // Bob
            KeyAgreement bKeyAgree = KeyAgreement.getInstance("DH");
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

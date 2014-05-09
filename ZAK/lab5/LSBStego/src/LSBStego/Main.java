package LSBStego;

/**
 * @author ZAK
 */
import java.awt.image.BufferedImage;
import java.io.*;
import javax.imageio.ImageIO;

/**
 * This program modifies bitmap image.
 * The header is untouched, so the images can be viewed after
 * processing in basic image viewer.
 */
public class Main {

    /**
     * Turns array of bytes into string
     *
     * @param buf Array of bytes to convert to hex string
     * @return	Generated hex string
     */
    public static String asHex(byte buf[]) {
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

    static byte[] getImageData(BufferedImage sourceImage) {

        int width = sourceImage.getWidth();
        int height = sourceImage.getHeight();
        int[] rgbs = new int[width * height];
        byte[] byteRGB = new byte[width * height * 3];

        sourceImage.getRGB(0, 0, width, height, rgbs, 0, width);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int idx = y * width + x;

                byte red = (byte) ((rgbs[idx] & 0x00ff0000) >> 16);
                byte green = (byte) ((rgbs[idx] & 0x0000ff00) >> 8);
                byte blue = (byte) (rgbs[idx] & 0x000000ff);

                byteRGB[idx * 3] = red;
                byteRGB[idx * 3 + 1] = green;
                byteRGB[idx * 3 + 2] = blue;
            }
        }
        return byteRGB;
    }

    static void setImageData(BufferedImage destImage, byte[] byteRGB) {

        int width = destImage.getWidth();
        int height = destImage.getHeight();
        int[] rgbs = new int[width * height];

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int idx = y * width + x;
                int r = (byteRGB[idx*3] << 16) & 0x00ff0000;
                int g = (byteRGB[idx*3+1] << 8) & 0x0000ff00;
                int b = byteRGB[idx*3+2] & 0x000000ff ;
                rgbs[idx] = 0xff000000+r+g+b;
            }
        }

        destImage.setRGB(0, 0, width, height, rgbs, 0, width);
    }

    /**
     * Sets/Clears given bit in byte
     * @param data Byte to be modified
     * @param bitNumber Bit number to be modified, 0 - LSB
     * @param value Value of the bit to be set: 0 - clears bit, any other sets bit
     * @return Modified byte or unmodified byte in case of wrong parameters
     */
    public static byte setBit(byte data, int bitNumber, int value) {
        byte a = 1;
        if (bitNumber < 0 || bitNumber > 7) {
            return data;
        }
        if (value == 0) {
            data = (byte) (data & ~(1 << bitNumber));
        } else {
            data = (byte) (data | (1 << bitNumber));
        }
        return data;
    }

    /**
     * Gets value of given bit in byte
     * @param data Byte to be read
     * @param bitNumber Bit number to be read, 0 - LSB
     * @return State of given bit
     */
    public static int getBit(byte data, int bitNumber) {
        int a;
        if (bitNumber < 0 || bitNumber > 7) {
            return data;
        }
        return (data >> bitNumber) & 0x01;
    }

    public static void modifyRGBData(byte[] byteRGB, byte[] data, int w, int h) {
        int x, y;
        int bitCounter = 0, byteCounter = 0;
        int indexR, indexG, indexB;

        // for even lines of image
        for (y = 0; y < h; y += 2) {

            // for every pixel in that line
            for (x = 0; x < w; x++) {

                // calculate indexes of RGB components of pixel at (x,y)
                indexR = y * (w * 3) + (x * 3);
                indexG = y * (w * 3) + (x * 3) + 1;
                indexB = y * (w * 3) + (x * 3) + 2;

                byteRGB[indexR] = setBit(byteRGB[indexR], 0, getBit(data[byteCounter], bitCounter++));
                //byteRGB[indexG] = setBit(byteRGB[indexG], 0, getBit(data[byteCounter], bitCounter++));
                //byteRGB[indexB] = setBit(byteRGB[indexB], 0, getBit(data[byteCounter], bitCounter++));
                if(bitCounter == 8)
                {
                    bitCounter = 0;
                    byteCounter++;
                    if(byteCounter >= data.length)
                        return;
                }
            }
        }
    }
    
    public static byte[] getRGBData(byte[] byteRGB, int w, int h) {
        int x, y;
        byte data = 0;
        byte[] output = new byte[w*h/3];
        int bitCounter = 0, byteCounter = 0;
        int indexR, indexG, indexB;

        // for even lines of image
        for (y = 0; y < h; y += 2) {

            // for every pixel in that line
            for (x = 0; x < w; x++) {

                // calculate indexes of RGB components of pixel at (x,y)
                indexR = y * (w * 3) + (x * 3);
                indexG = y * (w * 3) + (x * 3) + 1;
                indexB = y * (w * 3) + (x * 3) + 2;

                data = setBit(data, bitCounter++, getBit(byteRGB[indexR], 0));
                //byteRGB[indexG] = setBit(byteRGB[indexG], 0, getBit(data[byteCounter], bitCounter++));
                //byteRGB[indexB] = setBit(byteRGB[indexB], 0, getBit(data[byteCounter], bitCounter++));
                if(bitCounter == 8)
                {
                    bitCounter = 0;
                    output[byteCounter++] = data;
                }
            }
        }
        
        return output;
    }

    // Returns the contents of the file in a byte array.
    public static byte[] getFileData(String fileName) throws IOException {

        File file = new File(fileName);
        InputStream is = new FileInputStream(file);

        long length = file.length();

        if (length > Integer.MAX_VALUE) {
            // File is too large
            return null;
        }

        byte[] bytes = new byte[(int) length];

        int offset = 0;
        int numRead = 0;
        while (offset < bytes.length && (numRead = is.read(bytes, offset, bytes.length - offset)) >= 0) {
            offset += numRead;
        }

        if (offset < bytes.length) {
            throw new IOException("Could not successfuly read file " + file.getName());
        }

        is.close();
        return bytes;
    }

    public static void main(String[] args) throws Exception {

        String fileName = "linux";
        String format = "bmp";

        // Read input file
        File file = new File(fileName + "." + format);
        BufferedImage sourceImage = null;
        try {
            sourceImage = ImageIO.read(file);
        } catch (IOException ex) {
            System.out.println(ex.getMessage());
            System.exit(0);
        }

        // Get image data as an array of bytes RGBRGBRGB...
        byte[] byteRGB = getImageData(sourceImage);
        
        // read data file

        byte[] fileBytes;

        fileBytes = getFileData("example.txt");

        modifyRGBData(byteRGB, fileBytes, sourceImage.getWidth(), sourceImage.getHeight());
        //fileBytes = getRGBData(byteRGB, sourceImage.getWidth(), sourceImage.getHeight());

        setImageData(sourceImage, byteRGB);

        // Write results
        try {
            File outputfile = new File(fileName + "-output." + format);
            ImageIO.write(sourceImage, format, outputfile);
        } catch (IOException e) {
            System.out.println("Output error");
        }


        

        if (fileBytes != null) {
            System.out.println("Binary dump of a file: ");

            // for every byte in file...
            for (int i = 0; i < fileBytes.length; i++) {

                // ... print its ASCII representation ...
                System.out.print("" + (char) fileBytes[i] + " : ");

                // ... and prints its bits
                for (int j = 0; j < 8; j++) {
                    System.out.print("" + getBit(fileBytes[i], 7 - j));
                }
                System.out.println("");
            }
        }
    }
}

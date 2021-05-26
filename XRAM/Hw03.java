// XRAM hashing algorithm for encryption.

import java.io.File;
import java.io.FileNotFoundException;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;

public class Hw03
{
    public static void main(String[] args) throws FileNotFoundException
    {
        int i, data_as_int;

        // Read the file.
        File file = new File(args[0]);
        Scanner input = new Scanner(file);
        ArrayList<String> line = new ArrayList<String>();

        // Scan all lines from the file.
        while (input.hasNextLine())
        {
            line.add(input.nextLine());
        }

        // Execute all commands from the file.
        for (i = 0; i < line.size(); i++)
        {
            // Collect the word.
            String[] parsed = line.get(i).split(" ");

            // Collect the amount of letters in the string.
            int strlen = parsed[0].length();

            // Transfer the word into a list of bytes.
            byte[] bytes = parsed[0].getBytes(StandardCharsets.UTF_8);

            int hash = XRAM.XRAM(bytes, strlen);
            System.out.format("%10x:%s\n", hash, parsed[0]);
        }

        System.out.format("Input file processed.\n");
    }
}

class XRAM
{
    static int XRAM(byte[] data, int len)
    {
        int tempData = 0;
        int randVal1 = 0xbcde98ef; // aribtrary value
        int randVal2 = 0x7890face;
        int hashValue = 0xfa01bc96; // start seed value
        int roundedEnd = (len & 0xfffffffc); // Array d gets 4 byte blocks.

        for (int i = 0; i < roundedEnd; i = i + 4)
        {
            tempData = (data[i] & 0xff) | ((data[i + 1] & 0xff) << 8) | ((data[i + 2] & 0xff) << 16) | (data[i+3] << 24);
            tempData = (tempData * randVal1); // Mulitiply
            tempData =  (tempData << 12); // rotate left
            tempData = (tempData * randVal2); // Another Multiply
            hashValue = (hashValue ^ tempData);
            hashValue =  (hashValue << 13); // rotate left
            hashValue = hashValue * (5 + 0x46b6456e);
        }

        // Collect orphan input characters.
        if ((len & 0x03) == 3)
        {
            tempData = (data[roundedEnd + 2] & 0xff) << 16;
            len -= 1;
        }

        if ((len & 0x03) == 2)
        {
            tempData |= (data[roundedEnd + 1] & 0xff) << 8;
            len -= 1;
        }

        if ((len & 0x03) == 1)
        {
            tempData |= (data[roundedEnd] & 0xff);
            tempData = (tempData * randVal1); // Mulitiply
            tempData = Integer.rotateLeft(tempData, 14); // Rotate left 14 bits
            tempData = (tempData * randVal2);
            hashValue = (hashValue ^ tempData);
        }

        hashValue = hashValue ^ len; // XOR
        hashValue = hashValue & 0xb6acbe58; // AND
        hashValue = hashValue ^ hashValue >>> 13; // Rotate right
        hashValue = (hashValue * 0x53ea2b2c);
        hashValue = hashValue ^ hashValue >>> 16;

        // Return the 32 bit int hash.
        return hashValue;
    }
}

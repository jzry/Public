// Encrypt object.
public class Encrypter
{
    public static String encrypt(String str)
    {
        String num1, num2, num3, num4, finalStr;
        int temp;

        // Get character at nth index, then convert into int type.
        int index0int = Character.getNumericValue(str.charAt(0));
        int index1int = Character.getNumericValue(str.charAt(1));
        int index2int = Character.getNumericValue(str.charAt(2));
        int index3int = Character.getNumericValue(str.charAt(3));

        // Do encryption.
        int newIndex0 = ((index0int + 7) % 10);
        int newIndex1 = ((index1int + 7) % 10);
        int newIndex2 = ((index2int + 7) % 10);
        int newIndex3 = ((index3int + 7) % 10);

        // Swapping element 0 with 2.
        temp = newIndex0;
        newIndex0 = newIndex2;
        newIndex2 = temp;

        // Swapping element 1 with 3.
        temp = newIndex1;
        newIndex1 = newIndex3;
        newIndex3 = temp;

        // Transfer each number to string, then return string.
        num1 = Integer.toString(newIndex0);
        num2 = Integer.toString(newIndex1);
        num3 = Integer.toString(newIndex2);
        num4 = Integer.toString(newIndex3);

        // Concatenate indexes for final string.
        finalStr = num1 + num2 + num3 + num4;
        return finalStr;
    }
}
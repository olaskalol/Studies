import Krypto.desx.BitOperations;
import org.junit.Test;

import static org.junit.Assert.*;

public class BitOperationsTest {

    @Test
    public void Test() {
        var word1 = "slowo";
        char[] word_array = word1.toCharArray();
        byte[] byte_array = BitOperations.toByteArray(word_array);
        System.out.println(BitOperations.toBinaryString(byte_array));
        assertEquals(1, BitOperations.getBit(byte_array, 1));
        BitOperations.setBit(byte_array, 1, 0);
        assertEquals(0, BitOperations.getBit(byte_array, 1));

    }
}

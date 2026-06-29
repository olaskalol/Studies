package Krypto.desx;

import java.util.BitSet;

public class BitOperations {

    private BitOperations() {}

    public static BitSet toBitSet(char[] textList) {
        BitSet bitSet = new BitSet();
        int bitIndex = 0;
        for (char c : textList) {
            int ascii = (int) c;
            for (int j=7 ; j>=0 ; j--){
                boolean isBitOne = ((ascii>>j)&1)==1;
                if (isBitOne) {
                    bitSet.set(bitIndex);
                }
                bitIndex++;
            }

        }
        return bitSet;
    }

    public static byte[] toByteArray(char[] textList) {
        byte[] by = new byte[textList.length];
        for (int i = 0; i < textList.length; i++) {
            by[i] = (byte) textList[i];
        }
        return by;
    }

    public static int getBit(byte[] by, int idx) {
        int byteIndex = idx / 8;
        int bitIndex = idx % 8;
        int mask = 1 << (7 - bitIndex);

        if ((by[byteIndex] & mask) != 0) {
            return 1;
        }
        else {
            return 0;
        }
    }
    public static void setBit(byte[] by, int idx, int value) {

        if (value != 0 && value != 1) {
            throw new IllegalArgumentException("ToBitOperations: Wpisz 0 lub 1");
        }

        int byteIndex = idx / 8;
        int bitIndex = idx % 8;
        int mask = 1 << (7 - bitIndex);

        if (value == 1) {
            by[byteIndex] |= mask;
        }
        else {
            by[byteIndex] &= ~mask;
        }
    }

    public static String toBinaryString(byte[] bytes) {
        StringBuilder sb = new StringBuilder();
        for (byte b : bytes) {
            sb.append(Integer.toBinaryString((b & 0xFF) + 0x100).substring(1));
        }
        return sb.toString();
    }

}

package Krypto.desx;

import java.util.Arrays;
import java.util.Base64;
import java.util.List;

public class DESXAlgorithms {

    private DESXAlgorithms() {}

    private static byte[] subkeys = new byte[128];

    public static byte[] getSubkeys() {
        return subkeys;
    }

    public static byte[] getSubkey(int i) {
        return Arrays.copyOfRange(subkeys, i*8, i*8+8);
    }

    public static void addSubkey(byte[] subkey, int round) {
        System.arraycopy(subkey, 0, getSubkeys(), round*8, subkey.length);
    }

    public static byte[] XOR(byte[] t1, byte[] t2) {
        byte[] out_table = new byte[t1.length];
        for (int i=0; i < t1.length; i++) {
            out_table[i] = (byte) (t1[i] ^ t2[i]);
        }
        return out_table;
    }

    public static byte[] permutation(byte[] before_table, byte[] permutation_table) {
        byte[] result_table = new byte[permutation_table.length/8];
        for (int i=0; i< permutation_table.length ; i++){
            BitOperations.setBit(result_table, i, BitOperations.getBit(before_table, permutation_table[i]-1));
        }
        return result_table;
    }

    public static byte[] rotation(int rotation_number, byte[] table) {
        int real_length = table.length*8;
        byte[] whole_table = table;
        for (int j= 0; j<rotation_number; j++) {
            byte[] shifted_table = new byte[table.length];
            for (int i=0; i < real_length/2; i++){
                if(i==0){
                    BitOperations.setBit(shifted_table, (real_length/2)-1, BitOperations.getBit(whole_table, i));
                    BitOperations.setBit(shifted_table, real_length-1, BitOperations.getBit(whole_table, i+real_length/2));
                }
                else {
                    BitOperations.setBit(shifted_table, i-1, BitOperations.getBit(whole_table, i));
                    BitOperations.setBit(shifted_table, i-1 + real_length/2, BitOperations.getBit(whole_table, i+real_length/2));
                }
            }
            whole_table = shifted_table;
        }
        return whole_table;
    }

    public static byte[] expansion(byte[] R, byte[] key) {
        byte[] afterPerm = permutation(R, PermutationsConstants.EBOX);
        byte[] afterXOR = XOR(afterPerm, key);

        byte[] output = new byte[4]; // 4x8
        for (int i=0; i<8; i++) {
            int startPos = i * 6;

            int row = (BitOperations.getBit(afterXOR, startPos) << 1) | BitOperations.getBit(afterXOR, startPos + 5);
            int col = (BitOperations.getBit(afterXOR, startPos + 1) << 3) | (BitOperations.getBit(afterXOR, startPos + 2) << 2) | (BitOperations.getBit(afterXOR, startPos + 3) << 1) | BitOperations.getBit(afterXOR, startPos + 4);

            byte sbox = PermutationsConstants.S_BOXES[i][row][col]; // 8x4  '00001010' -> '00000001' -> '00000010'

            int outputStart = i * 4;
            BitOperations.setBit(output, outputStart, (sbox >> 3) & 1);
            BitOperations.setBit(output, outputStart + 1, (sbox >> 2) & 1);
            BitOperations.setBit(output, outputStart + 2, (sbox >> 1) & 1);
            BitOperations.setBit(output, outputStart + 3, sbox & 1);

        }
        return output;

    }

    public static byte[] padded(byte[] input) {
        int blockSize = 8;
        int paddingSize = blockSize - (input.length % blockSize);
        byte[] padded = new byte[input.length + paddingSize];
        System.arraycopy(input, 0, padded, 0, input.length);

        for (int i = input.length; i < padded.length - 1; i++) {
            padded[i] = (byte) 0x00;
        }
        padded[padded.length - 1] = (byte) paddingSize;
        return padded;
    }

    public static void subkeyGeneration(byte[] key2) {
        for (int i=0; i<16; i++) {
            int rotation_number = 2;
            if (java.util.List.of(1, 2, 9, 16).contains(i + 1)) {
                rotation_number = 1;
            }
            key2 = rotation(rotation_number, key2);
            byte[] new_key2 = permutation(key2, PermutationsConstants.COMPRESSION_TABLE);
            addSubkey(new_key2, i);
        }
    }

    public static byte[] EncryptBlock(byte[] key1, byte[] key2, byte[] key3, byte[] message) {

        // Wybielanie wstępne
        message = XOR(key1, message);

        // Wstępne permutacje klucza głównego i wiadomości
        key2 = permutation(key2, PermutationsConstants.PC1);
        message = permutation(message, PermutationsConstants.IP);

        byte[] L = Arrays.copyOfRange(message,0,message.length/2);
        byte[] R = Arrays.copyOfRange(message,message.length/2, message.length);

        subkeyGeneration(key2);

        // Rundy
        for (int i=0; i < 16; i++) {

            // Podklucz

            byte[] subkey = getSubkey(i);

            // Tekst jawny
            byte[] func = expansion(R, subkey);
            func = permutation(func, PermutationsConstants.PBOX);

            byte[] tmp = R;
            R = XOR(L, func);
            L = tmp;

        }

        byte[] result = new byte[8];
        System.arraycopy(R, 0, result, 0, 4);
        System.arraycopy(L, 0, result, 4, 4);

        message = result;
        message = permutation(message, PermutationsConstants.IPN1);

        message = XOR(key3, message);

        return message;
    }

    public static byte[] Encrypt(byte[] key1, byte[] key2, byte[] key3, byte[] message) {
        byte[] paddedMessage = padded(message);
        byte[] result = new byte[paddedMessage.length];

        for (int i=0; i < paddedMessage.length; i+=8) {
            byte[] block = Arrays.copyOfRange(paddedMessage,i,i+8);
            byte[] encryptedBlock = EncryptBlock(key1, key2, key3, block);
            System.arraycopy(encryptedBlock, 0, result, i, 8);
        }
        return result;
    }


    public static byte[] DecryptBlock(byte[] key1, byte[] key2, byte[] key3, byte[] encrypted_message) {

        // Wybielanie wstępne
        encrypted_message = XOR(key3, encrypted_message);

        // Wstępne permutacje klucza głównego i wiadomości
        encrypted_message = permutation(encrypted_message, PermutationsConstants.IP);

        byte[] L = Arrays.copyOfRange(encrypted_message,0,encrypted_message.length/2);
        byte[] R = Arrays.copyOfRange(encrypted_message,encrypted_message.length/2, encrypted_message.length);

        // Rundy
        for (int i=15; i >= 0; i--) {

            // Podklucz
            byte[] subkey = getSubkey(i);

            // Tekst jawny
            byte[] func = expansion(R, subkey);
            func = permutation(func, PermutationsConstants.PBOX);

            byte[] tmp = R;
            R = XOR(L, func);
            L = tmp;

        }

        byte[] result = new byte[8];
        System.arraycopy(R, 0, result, 0, 4);
        System.arraycopy(L, 0, result, 4, 4);

        encrypted_message = result;
        encrypted_message = permutation(encrypted_message, PermutationsConstants.IPN1);

        encrypted_message = XOR(key1, encrypted_message);

        return encrypted_message;
    }

    public static byte[] Decrypt(byte[] key1, byte[] key2, byte[] key3, byte[] message) {
        byte[] result = new byte[message.length];
        for (int i=0; i<message.length; i+=8) {
            byte[] block = Arrays.copyOfRange(message,i,i+8);
            byte[] decryptedBlock = DecryptBlock(key1, key2, key3, block);
            System.arraycopy(decryptedBlock, 0, result, i, 8);
        }
        int paddingSize = result[result.length - 1];
        return Arrays.copyOfRange(result, 0, result.length-paddingSize);
    }

}



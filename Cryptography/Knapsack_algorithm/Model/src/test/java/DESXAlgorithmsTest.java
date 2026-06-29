import Krypto.desx.BitOperations;
import Krypto.desx.DESXAlgorithms;
import Krypto.desx.PermutationsConstants;
import org.junit.Test;

import static org.junit.Assert.*;

public class DESXAlgorithmsTest {

    @Test
    public void testXOR() {
        byte[] table1 = {(byte) 0b10101010};
        byte[] table2 = {(byte) 0b11001100};
        byte[] expected = {(byte) 0b01100110};

        byte[] result = DESXAlgorithms.XOR(table1, table2);
        assertArrayEquals(expected, result);
    }

    @Test
    public void testPermutation() {
        byte[] input = {(byte) 0b10000000};
        byte[] miniTable = {8, 7, 6, 1, 2, 3, 4, 5};
        byte[] expected = {(byte) 0b00010000};
        byte[] result = DESXAlgorithms.permutation(input, miniTable);

        assertArrayEquals(expected, result);

    }

    @Test
    public void testExtremePermutations() {
        // --- SKRAJNY PRZYPADEK 1: Klonowanie bitu (Ekspansja) ---
        byte[] input1 = {(byte) 0b10000000};
        byte[] cloneTable = {1, 1, 1, 1, 1, 1, 1, 1};
        byte[] expected1 = {(byte) 0b11111111};
        byte[] result1 = DESXAlgorithms.permutation(input1, cloneTable);

        assertArrayEquals("Permutacja nie potrafi sklonować bitu!", expected1, result1);

        // --- SKRAJNY PRZYPADEK 2: Daleki skok i kompresja ---

        byte[] input2 = {(byte) 0b00000000, (byte) 0b00000001};
        byte[] jumpTable = {16, 1, 1, 1, 1, 1, 1, 1};
        byte[] expected2 = {(byte) 0b10000000};
        byte[] result2 = DESXAlgorithms.permutation(input2, jumpTable);

        assertArrayEquals("Permutacja gubi się przy skakaniu między bajtami!", expected2, result2);
    }

    @Test
    public void testLongPermutationIP() {
        // --- SKRAJNY PRZYPADEK 3: Pełna, 64-bitowa tablica IP z DES ---
        byte[] input = {
                (byte) 0b10000000, (byte) 0b00000000, (byte) 0b00000000, (byte) 0b00000000,
                (byte) 0b00000000, (byte) 0b00000000, (byte) 0b00000000, (byte) 0b00000001
        };
        byte[] longTable = PermutationsConstants.IP;

        byte[] expected = {
                (byte) 0b00000000, (byte) 0b00000000, (byte) 0b00000000, (byte) 0b10000000, // Pierwsze 4 bajty
                (byte) 0b00000001, (byte) 0b00000000, (byte) 0b00000000, (byte) 0b00000000  // Kolejne 4 bajty
        };
        byte[] result = DESXAlgorithms.permutation(input, longTable);
        assertArrayEquals("Pełna 64-bitowa permutacja IP pomieszała bity!", expected, result);
    }

    @Test
    public void testRotation() {

        byte[] input = new byte[7];
        int dlugoscc = input.length*8;
        BitOperations.setBit(input, 0, 1);
        BitOperations.setBit(input, 28, 1);
        byte[] result = DESXAlgorithms.rotation(1, input);

        assertEquals("Lewa połówka nie przesunęła pierwszego bitu na koniec (pozycja 27)",
                1, BitOperations.getBit(result, 27));
        assertEquals("Prawa połówka nie przesunęła pierwszego bitu na koniec (pozycja 55)",
                1, BitOperations.getBit(result, 55));
        assertEquals(0, BitOperations.getBit(result, 0));
        assertEquals(0, BitOperations.getBit(result, 28));
    }

    @Test
    public void testSubkeyGeneration() {
        byte[] inputKey = new byte[7];
        //byte[] subkey1 = DESXAlgorithms.subkey(1, inputKey);
        //assertEquals("Podklucz powinien mieć 48 bitów (6 bajtów) po tabeli kompresji PC-2!",
                //6, subkey1.length);
    }

    @Test
    public void testAdvancedRotationShiftByTwo() {
        // Tworzymy 56-bitowy klucz (7 bajtów)
        byte[] input = new byte[7];

        // --- LEWA POŁOWA (indeksy 0 - 27) ---
        // Zapalamy trzy pierwsze bity.
        BitOperations.setBit(input, 0, 1);
        BitOperations.setBit(input, 1, 1);
        BitOperations.setBit(input, 2, 1);

        // --- PRAWA POŁOWA (indeksy 28 - 55) ---
        // Zapalamy trzy pierwsze bity prawej połówki.
        BitOperations.setBit(input, 28, 1);
        BitOperations.setBit(input, 29, 1);
        BitOperations.setBit(input, 30, 1);

        // Wykonujemy przesunięcie o 2 w lewo!
        byte[] result1 = DESXAlgorithms.rotation(2, input);

        // SPRAWDZAMY LEWĄ POŁOWĘ:
        // Bit 0 powinien zawinąć się na sam koniec lewej połowy -> indeks 26
        // Bit 1 powinien zawinąć się za nim -> indeks 27
        // Bit 2 powinien przesunąć się po prostu o dwa w lewo -> indeks 0
        assertEquals("Bit 0 nie zawinął się poprawnie na pozycję 26", 1, BitOperations.getBit(result1, 26));
        assertEquals("Bit 1 nie zawinął się poprawnie na pozycję 27", 1, BitOperations.getBit(result1, 27));
        assertEquals("Bit 2 nie przesunął się na pozycję 0", 1, BitOperations.getBit(result1, 0));

        // SPRAWDZAMY PRAWĄ POŁOWĘ:
        // Bit 28 -> pozycja 54
        // Bit 29 -> pozycja 55
        // Bit 30 -> pozycja 28
        assertEquals("Bit 28 nie zawinął się poprawnie na pozycję 54", 1, BitOperations.getBit(result1, 54));
        assertEquals("Bit 29 nie zawinął się poprawnie na pozycję 55", 1, BitOperations.getBit(result1, 55));
        assertEquals("Bit 30 nie przesunął się na pozycję 28", 1, BitOperations.getBit(result1, 28));

        // Upewnijmy się, że na pozycji 1 jest teraz zero (bo bit 3 przesunął się tam z jedynką? Nie, bit 3 był zerem!)
        assertEquals("Pozycja 1 powinna być pusta (0)", 0, BitOperations.getBit(result1, 1));
    }

    @Test
    public void testSubkeyRound3ShiftByTwo() {
        byte[] input = new byte[7];

        // Wybieramy do testu Rundę 3 (zgodnie z logiką DES, rundy 1,2,9,16 mają obrót o 1, reszta o 2)
        // Twoja funkcja automatycznie ustawi rotation_number na 2.

        // 1. Zagadka dla pierwszego bitu podklucza.
        // PC-2 zaczyna się od '14' (indeks 13). Żeby po podwójnej rotacji w lewo jedynka znalazła się
        // na indeksie 13, musi wystartować z indeksu 15 (15 - 2 = 13).
        BitOperations.setBit(input, 15, 1);

        // 2. Zagadka dla drugiego bitu podklucza.
        // PC-2 ma na drugim miejscu '17' (indeks 16). Przed rotacją: 16 + 2 = 18.
        BitOperations.setBit(input, 18, 1);

        // 3. Zagadka dla piątego bitu podklucza.
        // PC-2 na piątym miejscu (indeks 4) ma cyfrę '1' (czyli szuka bitu na indeksie 0).
        // Co musi być w kluczu, żeby po obrocie o 2 znaleźć się na indeksie 0? Bit nr 2!
        BitOperations.setBit(input, 2, 1);

        // Odpalamy kombajn generujący podklucz!
        //byte[] subkey = DESXAlgorithms.subkey(3, input);

        // Weryfikacja:
        //assertEquals("Długość podklucza musi wynosić 6 bajtów (48 bitów)", 6, subkey.length);

        //assertEquals("Pierwszy bit gotowego podklucza (indeks 0) nie wynosi 1!", 1, BitOperations.getBit(subkey, 0));
        //assertEquals("Drugi bit gotowego podklucza (indeks 1) nie wynosi 1!", 1, BitOperations.getBit(subkey, 1));
        //assertEquals("Piąty bit gotowego podklucza (indeks 4) nie wynosi 1!", 1, BitOperations.getBit(subkey, 4));

        // Sprawdzamy, czy "śmieci" nie pojawiły się w nieodpowiednich miejscach (np. 3. bit powinien być zerem)
        //assertEquals("Trzeci bit podklucza powinien być 0", 0, BitOperations.getBit(subkey, 2));
    }

    @Test
    public void testExpansion() {
        byte[] R = new byte[4];
        byte[] key = new byte[6];

        byte[] expected = {
                (byte) 0b11101111, // S1(14) i S2(15)
                (byte) 0b10100111, // S3(10) i S4(7)
                (byte) 0b00101100, // S5(2) i S6(12)
                (byte) 0b01001101  // S7(4) i S8(13)
        };

        byte[] result = DESXAlgorithms.expansion(R, key);

        assertArrayEquals("S-Boxy nie zwróciły poprawnego wyniku dla zerowego wejścia!", expected, result);
    }
}

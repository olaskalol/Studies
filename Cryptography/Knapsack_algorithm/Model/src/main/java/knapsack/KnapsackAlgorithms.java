package knapsack;

import Krypto.desx.BitOperations;
import java.math.BigInteger;
import java.security.SecureRandom;

public class KnapsackAlgorithms {

    private KnapsackAlgorithms() {}

    private static final SecureRandom random = new SecureRandom();

    // Szyfrowanie odbywa się za pomocą klucza publicznego
    public static BigInteger encryptBlock(BigInteger[] publicKey, byte[] block) {
        BigInteger sum = BigInteger.ZERO;
        // Iterujemy przez każdy bit w tekście jawnym
        for (int i = 0; i < publicKey.length; i++) {
            // Jeżeli ten bit jest ustawiony na 1 to do zaszyfrowanego bloku dodajemy wartość klucza publicznego w tym miejscu
            if (BitOperations.getBit(block, i) == 1) {
                sum = sum.add(publicKey[i]);
            }
        }
        return sum;
    }

    // Odszyfrowywanie odbywa się za pomocą klucza prywatnego
    public static byte[] decryptBlock(BigInteger cryptogram, BigInteger[] privateKey, BigInteger m, BigInteger wInverse) {
        // Wyznaczamy w^-1, które spelnia w^-1 * w = 1 mod m. Na podstawie tego obliczmy k, które jest cofnięciem dodania do siebie wag klucza publicznego i pomieszania ich przez mnożnik w
        BigInteger k = cryptogram.multiply(wInverse).mod(m);
        // Robimy miejsce na wynik, w którym ilość bajtów na pewno będzie zaokrąglona w górę
        byte[] result = new byte[(privateKey.length + 7) / 8];

        // w kluczu prywatnym iterujemy od końca, ponieważ najpierw sprawdzamy największe liczby w tym wektorze superrosnącym
        for (int i = privateKey.length - 1; i >= 0; i--) {
            // jeżeli k >= obecnej wadze to ustawiamy na tym miejscu 1 i odejmujemy tą wartość bo szukamy reszty
            if (k.compareTo(privateKey[i]) >= 0) {
                BitOperations.setBit(result, i, 1);
                k = k.subtract(privateKey[i]);
            } else { // jeżeli k < obecnej wagi to nie ma jak tego użyć, więc ustawiamy ten bit na 0
                BitOperations.setBit(result, i, 0);
            }
        }
        return result;
    }

    // Klucz publiczny jest tworzony na podstawie klucza prywatnego, w i m
    public static BigInteger[] generatePublicKey(BigInteger[] privateKey, BigInteger w, BigInteger m) {
        // Musi mieć taką samą dlugosc co klucz prywatny
        BigInteger[] publicKey = new BigInteger[privateKey.length];
        for (int i = 0; i < privateKey.length; i++) {
            // Klucz Publiczny[i] = (Klucz Prywatny[i] * W) mod M
            publicKey[i] = privateKey[i].multiply(w).mod(m);
        }
        return publicKey;
    }


    // Generowanie klucza prywatnego, losujemy kolejne wartości (ich ilość zależy od n), gdzie każda kolejna musi być wieksza od sumy poprzednich.
    public static BigInteger[] generateSuperincreasingKey(int length) {
        BigInteger[] privateKey = new BigInteger[length];
        BigInteger sum = BigInteger.ZERO;

        for (int i = 0; i < length; i++) {
            BigInteger randomBump = new BigInteger(10, random).add(BigInteger.ONE);
            BigInteger nextValue = sum.add(randomBump);

            privateKey[i] = nextValue;
            sum = sum.add(nextValue);
        }
        return privateKey;
    }
    // M - musi być większe od sumy wag w kluczu prywatnym, więc obliczamy sume wag oraz dodajemy liczbę.
    public static BigInteger generateM(BigInteger[] privateKey) {
        BigInteger sum = BigInteger.ZERO;
        //zliczanie sumy
        for (BigInteger weight : privateKey) {
            sum = sum.add(weight);
        }
        //sprawienie, że M jest większe od sumy wag (losowane są od 1 do 1024 przez to jak działa BigInteger)
        BigInteger randomBump = new BigInteger(10, random).add(BigInteger.ONE);
        return sum.add(randomBump);
    }
    // W - musi być większe od M/2(większe od 1), mniejsze od M i względnie pierwsze z M
    public static BigInteger generateW(BigInteger m) {
        BigInteger w;
        BigInteger halfM = m.shiftRight(1);
        //losujemy W dopóki nam nie spełni warunków
        do {
            w = new BigInteger(m.bitLength(), random);
        } while (w.compareTo(halfM) <= 0 || w.compareTo(m) >= 0 || !w.gcd(m).equals(BigInteger.ONE));

        return w;
    }

    // Używane do odszyfrowania
    public static BigInteger calculateInverse(BigInteger w, BigInteger m) {
        return w.modInverse(m);
    }
}
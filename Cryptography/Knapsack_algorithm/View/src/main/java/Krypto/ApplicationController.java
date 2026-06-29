package Krypto;

import Krypto.desx.DESXAlgorithms;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.stage.FileChooser;
import javafx.stage.Stage;

import java.io.File;
import java.io.IOException;
import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.security.SecureRandom;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public class ApplicationController {
    protected Scene scene;
    protected Stage stage;
    protected Parent root;
    private byte[] loadedPlainBytes = null;
    private byte[] loadedCipherBytes = null;
    private byte[] readyToSavePlain = null;
    private byte[] readyToSaveCipher = null;

    @FXML protected Button chooseFileB, saveFileBeforeDESX, chooseFileA, saveFileAfterDESX;
    @FXML protected Button encryptButton, decryptButton, RightX, LeftX, generateButton;
    @FXML protected Label title, sourceDecrypted, sourceEncrypted;
    @FXML protected TextArea fileStatus, TextAreaB, TextAreaA;
    @FXML protected TextField key1, key2, key3;

    @FXML protected TextField knapN;
    @FXML protected TextArea genPubKey;
    @FXML protected TextArea genPrivKey;

    @FXML protected TextArea encPubKey;
    @FXML protected TextArea encInput;
    @FXML protected TextArea encOutput;

    @FXML protected TextArea decPrivKey;
    @FXML protected TextArea decInput;
    @FXML protected TextArea decOutput;

    public ApplicationController() {}

    @FXML public void initialize() {}


    private void showStatus(String message) {
        fileStatus.setText(message);
        fileStatus.setVisible(true);
    }

    @FXML void handleSelectFileBeforeDESX(ActionEvent event) throws IOException {
        FileChooser fileChooser = new FileChooser();
        File selectedFile = fileChooser.showOpenDialog(((Node)event.getSource()).getScene().getWindow());
        if (selectedFile != null) {
            sourceDecrypted.setText(selectedFile.getName());
            this.loadedPlainBytes = Files.readAllBytes(selectedFile.toPath());
            showStatus("Wczytano plik do zaszyfrowania: " + selectedFile.getName());
        }
    }

    @FXML void handleSelectFileAfterDESX(ActionEvent event) throws IOException {
        FileChooser fileChooser = new FileChooser();
        File selectedFile = fileChooser.showOpenDialog(((Node)event.getSource()).getScene().getWindow());
        if (selectedFile != null) {
            sourceEncrypted.setText(selectedFile.getName());
            this.loadedCipherBytes = Files.readAllBytes(selectedFile.toPath());
            showStatus("Wczytano kryptogram z dysku: " + selectedFile.getName());
        }
    }

    @FXML void handleSaveFileAfterDESX(ActionEvent event) throws IOException {
        if(readyToSaveCipher == null) { showStatus("Brak danych do zapisania!"); return; }
        FileChooser fileChooser = new FileChooser();
        File file = fileChooser.showSaveDialog(((Node)event.getSource()).getScene().getWindow());
        if (file != null) {
            Files.write(file.toPath(), readyToSaveCipher);
            showStatus("Kryptogram zapisany pomyślnie.");
        }
    }

    @FXML void handleSaveFileBeforeDESX(ActionEvent event) throws IOException {
        if(readyToSavePlain == null) { showStatus("Brak danych do zapisania!"); return; }
        FileChooser fileChooser = new FileChooser();
        File file = fileChooser.showSaveDialog(((Node)event.getSource()).getScene().getWindow());
        if (file != null) {
            Files.write(file.toPath(), readyToSavePlain);
            showStatus("Odzyskany plik zapisany pomyślnie.");
        }
    }

    @FXML
    void handleClearPathB(ActionEvent event) {
        loadedPlainBytes = null;
        sourceDecrypted.setText("");
        LeftX.setVisible(false);
        showStatus("Wyczyszczono plik jawny");
    }

    @FXML
    void handleClearPathA(ActionEvent event) {
        loadedCipherBytes = null;
        sourceEncrypted.setText("");
        RightX.setVisible(false);
        showStatus("Wyczyszczono plik zaszyfrowany");
    }



    @FXML
    void handleKnapGenerateKeys(ActionEvent event) {
        try {
            int blockSize = Integer.parseInt(knapN.getText().trim());
            if (blockSize <= 0) {
                showStatus("BŁĄD: Długość wektora n musi być większa od 0!");
                return;
            }

            BigInteger[] privateKey = knapsack.KnapsackAlgorithms.generateSuperincreasingKey(blockSize);
            BigInteger m = knapsack.KnapsackAlgorithms.generateM(privateKey);
            BigInteger w = knapsack.KnapsackAlgorithms.generateW(m);
            BigInteger[] publicKey = knapsack.KnapsackAlgorithms.generatePublicKey(privateKey, w, m);

            String pubKeyStr = Arrays.stream(publicKey)
                    .map(BigInteger::toString).collect(Collectors.joining(", "));

            String privVector = Arrays.stream(privateKey)
                    .map(BigInteger::toString).collect(Collectors.joining(", "));
            String privKeyStr = w.toString() + " ; " + m.toString() + " ; " + privVector;

            genPubKey.setText(pubKeyStr);
            genPrivKey.setText(privKeyStr);
            showStatus("Klucze wygenerowane (Dla n=" + blockSize + ")");
        } catch (NumberFormatException e) {
            showStatus("BŁĄD: Wpisz poprawną liczbę całkowitą dla długości n!");
        } catch (Exception e) {
            showStatus("Błąd generatora: " + e.getMessage());
        }
    }

    @FXML
    void handleKnapEncrypt(ActionEvent event) {
        try {
            String pubKeyText = encPubKey.getText();
            if (pubKeyText.isBlank()) { showStatus("Wklej klucz publiczny odbiorcy!"); return; }

            BigInteger[] publicKey = Arrays.stream(pubKeyText.split(","))
                    .map(String::trim).map(BigInteger::new).toArray(BigInteger[]::new);
            int blockSize = publicKey.length;

            byte[] inputBytes;
            if (loadedPlainBytes != null) {
                inputBytes = loadedPlainBytes;
            } else {
                String plainText = encInput.getText();
                if (plainText.isBlank()) { showStatus("Wpisz tekst lub ładuj plik."); return; }
                inputBytes = plainText.getBytes(StandardCharsets.UTF_8);
            }

            List<BigInteger> cipher = new ArrayList<>();
            for (int i = 0; i < inputBytes.length * 8; i += blockSize) {
                byte[] blockBits = new byte[(blockSize + 7) / 8];
                for (int j = 0; j < blockSize; j++) {
                    if (i + j < inputBytes.length * 8) {
                        Krypto.desx.BitOperations.setBit(blockBits, j, Krypto.desx.BitOperations.getBit(inputBytes, i + j));
                    }
                }
                cipher.add(knapsack.KnapsackAlgorithms.encryptBlock(publicKey, blockBits));
            }

            String cipherString = cipher.stream().map(BigInteger::toString).collect(Collectors.joining(", "));
            readyToSaveCipher = cipherString.getBytes(StandardCharsets.UTF_8);

            encOutput.setText(cipherString);
            showStatus("Zaszyfrowano pomyślnie!");

        } catch (Exception e) {
            showStatus("Błąd szyfrowania: Sprawdź poprawność klucza publicznego.");
        }
    }

    @FXML
    void handleKnapDecrypt(ActionEvent event) {
        try {
            String privKeyText = decPrivKey.getText();
            String[] keyParts = privKeyText.split(";");
            if (keyParts.length != 3) {
                showStatus("Błąd: Zły format klucza prywatnego! Wymagane: W ; M ; waga1, waga2...");
                return;
            }

            BigInteger w = new BigInteger(keyParts[0].trim());
            BigInteger m = new BigInteger(keyParts[1].trim());
            BigInteger[] privateKey = Arrays.stream(keyParts[2].split(","))
                    .map(String::trim).map(BigInteger::new).toArray(BigInteger[]::new);

            if (!isSuperincreasing(privateKey)) {
                showStatus("Błąd: Wektor nie jest superrosnący!");
                return;
            }
            BigInteger wInverse = knapsack.KnapsackAlgorithms.calculateInverse(w, m);
            int blockSize = privateKey.length;

            String inputText;
            if (loadedCipherBytes != null) {
                inputText = new String(loadedCipherBytes, StandardCharsets.UTF_8);
            } else {
                inputText = decInput.getText();
                if (inputText.isBlank()) { showStatus("Wklej kryptogram do odszyfrowania."); return; }
            }

            List<BigInteger> cipher = Arrays.stream(inputText.split(","))
                    .map(String::trim).map(BigInteger::new)
                    .collect(Collectors.toList());

            int totalBits = cipher.size() * blockSize;
            byte[] resultBytes = new byte[(totalBits + 7) / 8];

            for (int i = 0; i < cipher.size(); i++) {
                byte[] decryptedBlock = knapsack.KnapsackAlgorithms.decryptBlock(cipher.get(i), privateKey, m, wInverse);
                for (int j = 0; j < blockSize; j++) {
                    int bit = Krypto.desx.BitOperations.getBit(decryptedBlock, j);
                    if (i * blockSize + j < totalBits) {
                        Krypto.desx.BitOperations.setBit(resultBytes, i * blockSize + j, bit);
                    }
                }
            }

            readyToSavePlain = resultBytes;
            if (loadedCipherBytes != null) {
                decOutput.setText("Odszyfrowano plik binarnego formatu. Użyj przycisku 'Zapisz odzyskany plik', aby zapisać go na dysku.");
                showStatus("Odszyfrowano plik pomyślnie! Czeka na zapis.");
            } else {
                decOutput.setText(new String(resultBytes, StandardCharsets.UTF_8).trim());
                showStatus("Odszyfrowano pomyślnie! Tekst jawny został odzyskany.");
            }

        } catch (Exception e) {
            showStatus("Błąd deszyfrowania: Sprawdź format klucza i kryptogramu.");
            e.printStackTrace();
        }
    }

    private boolean isSuperincreasing(BigInteger[] key) {
        if (key == null || key.length == 0) return false;
        BigInteger sum = key[0];
        for (int i = 1; i < key.length; i++) {
            if (key[i].compareTo(sum) <= 0) return false;
            sum = sum.add(key[i]);
        }
        return true;
    }

}
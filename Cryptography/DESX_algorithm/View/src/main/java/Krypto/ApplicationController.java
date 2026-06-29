package Krypto;

import Krypto.desx.DESXAlgorithms;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
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
import java.nio.file.Files;
import java.security.SecureRandom;

public class ApplicationController {
    protected Scene scene;
    protected Stage stage;
    protected Parent root;
    private byte[] loadedPlainBytes = null;
    private byte[] loadedCipherBytes = null;
    private byte[] readyToSavePlain = null;
    private byte[] readyToSaveCipher = null;

    @FXML
    protected Button chooseFileB;
    @FXML
    protected Button saveFileBeforeDESX;
    @FXML
    protected Button chooseFileA;
    @FXML
    protected Button saveFileAfterDESX;
    @FXML
    protected Button encryptButton;
    @FXML
    protected Button decryptButton;
    @FXML
    protected Button RightX;
    @FXML
    protected Button LeftX;
    @FXML
    protected Button generateButton;

    @FXML
    protected Label title;
    @FXML
    protected TextArea fileStatus;
    @FXML
    protected Label sourceDecrypted;
    @FXML
    protected Label sourceEncrypted;

    @FXML
    protected TextField key1;
    @FXML
    protected TextField key2;
    @FXML
    protected TextField key3;
    //javafx automatycznie obsluguje wpisywanie rzeczy z klawiatury
    @FXML
    protected TextArea TextAreaB;
    @FXML
    protected TextArea TextAreaA;

    @FXML
    public void initialize() {
    }

    public ApplicationController() {

    }
    public FXMLLoader getFxmlLoader(String Scene) {
        var loader = new FXMLLoader(getClass().getResource(scene + "-view.fxml"));
        try {
            root = loader.load();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        return loader;
    }

    protected void refreshScene(ActionEvent event) {
        stage = (Stage)((Node)event.getSource()).getScene().getWindow();
        scene = new Scene(root);
        stage.setScene(scene);
        stage.show();

    }

    private void showStatus(String message) {
        fileStatus.setText(message);
        fileStatus.setVisible(true);
    }

    @FXML
    void handleEncrypt(ActionEvent event) {
        try {
            byte[] inputBytes;
            if (loadedPlainBytes != null) {
                inputBytes = loadedPlainBytes;
            } else {
                String plainText = TextAreaB.getText();
                if (plainText == null || plainText.isEmpty()) {
                    showStatus("Brak tekstu/pliku do zaszyfrowania");
                    return;
                }
                inputBytes = plainText.getBytes(java.nio.charset.StandardCharsets.UTF_8);
            }

            byte[] k1 = java.util.Arrays.copyOf(key1.getText().getBytes(java.nio.charset.StandardCharsets.UTF_8), 8);
            byte[] k2 = java.util.Arrays.copyOf(key2.getText().getBytes(java.nio.charset.StandardCharsets.UTF_8), 8);
            byte[] k3 = java.util.Arrays.copyOf(key3.getText().getBytes(java.nio.charset.StandardCharsets.UTF_8), 8);

            byte[] encryptedBytes = DESXAlgorithms.Encrypt(k1, k2, k3, inputBytes);
            readyToSaveCipher = encryptedBytes;

            if (loadedPlainBytes == null) {
                String encrypted = java.util.Base64.getEncoder().encodeToString(encryptedBytes);
                TextAreaA.setText(encrypted);
                showStatus("Zaszyfrowano pomyślnie!");
            }
            else {
                showStatus("Zaszyfrowano pomyślnie! Szyfrogram czeka na pobranie");
            }


        } catch (Exception e) {
            showStatus("Błąd szyfrowania: " + e.getMessage());
        }
    }

    @FXML
    void handleDecrypt(ActionEvent event) {
        try {
            byte[] encryptedBytes;

            if (loadedCipherBytes != null) {
                encryptedBytes = loadedCipherBytes;
            } else {
                String afterText = TextAreaA.getText();
                if (afterText == null || afterText.isEmpty()) {
                    showStatus("Brak szyfrogramu");
                    return;
                }
                encryptedBytes = java.util.Base64.getDecoder().decode(afterText);
            }

            byte[] k1 = java.util.Arrays.copyOf(key1.getText().getBytes(java.nio.charset.StandardCharsets.UTF_8), 8);
            byte[] k2 = java.util.Arrays.copyOf(key2.getText().getBytes(java.nio.charset.StandardCharsets.UTF_8), 8);
            byte[] k3 = java.util.Arrays.copyOf(key3.getText().getBytes(java.nio.charset.StandardCharsets.UTF_8), 8);

            byte[] decryptedBytes = DESXAlgorithms.Decrypt(k1, k2, k3, encryptedBytes);
            readyToSavePlain = decryptedBytes;

            if (loadedCipherBytes == null) {
                String plainText = new String(decryptedBytes, java.nio.charset.StandardCharsets.UTF_8);
                TextAreaB.setText(plainText);
                showStatus("Odszyfrowano pomyślnie!");
            }
            else {
                showStatus("Odszyfrowano pomyślnie! Odszyfrowany plik czeka na pobranie");
            }

        } catch (Exception e) {
            showStatus("Błąd deszyfrowania! Sprawdź klucze.");
        }
    }

    @FXML
    void handleGenerate(ActionEvent event) {
        key1.setText(generate8CharKey());
        key2.setText(generate8CharKey());
        key3.setText(generate8CharKey());
    }

    @FXML
    void handleSelectFileBeforeDESX(ActionEvent event) throws IOException {
        FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("Wybierz plik do szyfrowania");
        File selectedFile = fileChooser.showOpenDialog(chooseFileB.getScene().getWindow());
        if (selectedFile != null) {
            String path = selectedFile.getAbsolutePath();
            sourceDecrypted.setText(path);
            LeftX.setVisible(true);
            this.loadedPlainBytes = Files.readAllBytes(selectedFile.toPath());
        }
    }
    @FXML
    void handleSelectFileAfterDESX(ActionEvent event) throws IOException {
        FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("Wybierz plik do odszyfrowania");
        File selectedFile = fileChooser.showOpenDialog(chooseFileA.getScene().getWindow());
        if (selectedFile != null) {
            String path = selectedFile.getAbsolutePath();
            sourceEncrypted.setText(path);
            RightX.setVisible(true);
            this.loadedCipherBytes = Files.readAllBytes(selectedFile.toPath());
        }
    }
   @FXML
    void handleSaveFileAfterDESX(ActionEvent event) throws IOException {
       FileChooser fileChooser = new FileChooser();
       fileChooser.setTitle("Zapisz wynik szyfrowania");
       fileChooser.getExtensionFilters().addAll(
               new FileChooser.ExtensionFilter("Wszystkie pliki", "*.*")
       );
       File file = fileChooser.showSaveDialog(saveFileAfterDESX.getScene().getWindow());
       if (file != null) {
           Files.write(file.toPath(), readyToSaveCipher);
           showStatus("Plik zapisany w: " + file.getAbsolutePath());
       }

    }

    @FXML
    void handleSaveFileBeforeDESX(ActionEvent event) throws IOException {
        FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("Zapisz wynik odszyfrowania");
        fileChooser.getExtensionFilters().addAll(
                new FileChooser.ExtensionFilter("Wszystkie pliki", "*.*")
        );
        File file = fileChooser.showSaveDialog(saveFileBeforeDESX.getScene().getWindow());
        if (file != null) {
            Files.write(file.toPath(), readyToSavePlain);
            showStatus("Plik zapisany w: " + file.getAbsolutePath());
        }

    }
    @FXML
    void handleClearPathB(ActionEvent event) {
        loadedPlainBytes = null;
        sourceDecrypted.setText("");
        LeftX.setVisible(false);
        showStatus("Wyczyszczono plik odszyfrowany");
    }

    @FXML
    void handleClearPathA(ActionEvent event) {
        loadedCipherBytes = null;
        sourceEncrypted.setText("");
        RightX.setVisible(false);
        showStatus("Wyczyszczono plik zaszyfrowany");
    }
    private static final String chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    private static final SecureRandom random = new SecureRandom();

    public static String generate8CharKey() {
        StringBuilder sb = new StringBuilder(8);
        for (int i = 0; i < 8; i++) {
            int index = random.nextInt(chars.length());
            sb.append(chars.charAt(index));
        }
        return sb.toString();
    }

}

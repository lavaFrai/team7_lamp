import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Random;
import java.util.regex.Pattern;

public class SignGeneratorApp extends Frame implements WindowListener {
    private String integrated_certificate = "g5jsb8be6xbek066ertm76bw0xeztlja5b38hdke";
    Label header;
    TextField uid_field;
    TextField cert_field;
    Button gen_button;
    Button do_request_button;
    TextArea result_area;
    String version = "1.0.0a";

    public SignGeneratorApp() {
        init_ui();
    }

    public void run() {
        setVisible(true);
    }

    void init_ui() {
        setLayout(null);

        setTitle("WebLamp sign gen");
        setResizable(false);
        setSize(320, 440);
        addWindowListener(this);

        header = new Label("Web Lamp sign gen");
        header.setFont(new Font("Arial Unicode MS", java.awt.Font.PLAIN, 32));
        header.setBounds(10, 30, 480, 40);
        add(header);

        // UID
        Label uid_label = new Label("UID:");
        uid_label.setBounds(20, 80, 35, 22);
        add(uid_label);

        uid_field = new TextField();
        uid_field.setBounds(80, 80, 90, 22);
        add(uid_field);

        Button gen_random_uid_button = new Button("Gen random");
        gen_random_uid_button.setBounds(175, 80, 80, 22);
        button_font(gen_random_uid_button);
        gen_random_uid_button.addActionListener((ActionEvent ae) -> {
            uid_field.setText(Integer.toString(Math.abs(new Random().nextInt())));
        });
        add(gen_random_uid_button);

        // Cert
        Label cert_label = new Label("Cert:");
        cert_label.setBounds(20, 110, 35, 22);
        add(cert_label);

        cert_field = new TextField();
        cert_field.setBounds(80, 110, 90, 22);
        add(cert_field);

        // Cert controls
        Button load_cert_button = new Button("Load file");
        load_cert_button.setBounds(175, 110, 80, 22);
        button_font(load_cert_button);
        load_cert_button.addActionListener((ActionEvent ae) -> {
            FileDialog fd = new FileDialog(this, "Certificate", FileDialog.LOAD);
            fd.setVisible(true);
            if (fd.getFile() == null) {
                return;
            } else {
                String path = fd.getDirectory() + fd.getFile();
                File f = new File(path);
                try {
                    Path _path = Paths.get(path);
                    if (Files.size(_path) < 2 * 1024 * 1024) {
                        String text = Files.readString(_path);
                        System.out.println(text);
                        cert_field.setText(text);
                    } else {
                        JOptionPane.showMessageDialog(this, "File too lage. Maximum size is 2M.", "Error",
                                JOptionPane.ERROR_MESSAGE);
                    }
                } catch (IOException e) {
                    JOptionPane.showMessageDialog(this, "File access error.", "Error",
                            JOptionPane.ERROR_MESSAGE);
                }
            }
        });
        add(load_cert_button);

        Button use_integred_cert_button = new Button("Use built-in");
        use_integred_cert_button.addActionListener((ActionEvent ae) -> {
            cert_field.setText(integrated_certificate);
        });

        use_integred_cert_button.setBounds(175, 140, 80, 22);
        button_font(use_integred_cert_button);
        add(use_integred_cert_button);

        // Result text area
        // result_area = new TextArea("*here will be keys*", 1000000000, 1000000000, TextArea.SCROLLBARS_VERTICAL_ONLY);
        result_area = new TextArea();
        result_area.setBounds(20, 170, 270, 200);
        add(result_area);

        // Generate button
        gen_button = new Button("Generate");
        gen_button.addActionListener((ActionEvent ae) -> {
            genSign();
            do_request_button.setEnabled(true);
        });
        gen_button.setFont(new Font("Arial Unicode MS", java.awt.Font.PLAIN, 12));
        gen_button.setBounds(220, 400, 80, 22);
        add(gen_button);

        // Do request
        do_request_button = new Button("Do request");
        do_request_button.addActionListener((ActionEvent ae) -> {
            openDoRequest();
        });
        do_request_button.setEnabled(false);
        do_request_button.setFont(new Font("Arial Unicode MS", java.awt.Font.PLAIN, 12));
        do_request_button.setBounds(120, 400, 80, 22);
        add(do_request_button);

        // Exit button
        Button exit_button = new Button("Exit");
        exit_button.addActionListener((ActionEvent ae) -> {
            System.exit(0);
        });
        exit_button.setFont(new Font("Arial Unicode MS", java.awt.Font.PLAIN, 12));
        exit_button.setBounds(20, 400, 80, 22);
        add(exit_button);
    }

    private void button_font(Button b) {
        b.setFont(new Font("Arial Unicode MS", Font.PLAIN, 12));
    }

    void genSign() {
        int uid;
        String uid_s = uid_field.getText();
        String cert = cert_field.getText();

        if (uid_s.length() == 0) {
            result_area.setText("UID must be filled");
            return;
        }

        try { uid = Integer.parseInt(uid_s); }
        catch (NumberFormatException e) {
            result_area.setText("UID must be int");
            return;
        }

        if (cert.length() == 0) {
            result_area.setText("Certificate must be filled");
            return;
        }

        SignGeneratorKotlin sign = new SignGeneratorKotlin(cert);

        StringBuilder result_string = new StringBuilder();
        result_string.append("WebLamp private key\n");
        result_string.append("Keys version v");
        result_string.append(version);
        result_string.append("\n\n");

        result_string.append("Certificate hash: ");
        result_string.append(sign.getCertCache());
        result_string.append("\n\n");
        result_string.append("UID: ");
        result_string.append(uid);
        result_string.append("\n");

        result_string.append("Private key: ");
        result_string.append(sign.getPrivateKey(uid));
        result_string.append("\n\n");

        result_area.setText(result_string.toString());
    }

    void openDoRequest() {
        DoRequestApp doRequestApp = new DoRequestApp( this );
        doRequestApp.run();
    }

    @Override
    public void windowOpened(WindowEvent e) {

    }

    @Override
    public void windowClosing(WindowEvent e) {
        System.exit(0);
    }

    @Override
    public void windowClosed(WindowEvent e) {

    }

    @Override
    public void windowIconified(WindowEvent e) {

    }

    @Override
    public void windowDeiconified(WindowEvent e) {

    }

    @Override
    public void windowActivated(WindowEvent e) {

    }

    @Override
    public void windowDeactivated(WindowEvent e) {

    }
}

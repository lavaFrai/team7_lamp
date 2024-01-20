import org.apache.commons.codec.digest.DigestUtils;

public class SignGenerator {
    String cert;

    public SignGenerator(String _cert) {
        cert = _cert;
    }

    public String getPrivateKey(int uid) {
        String key_data = "<" + uid + ">:<" + cert + ">";

        return DigestUtils.sha256Hex(key_data).toUpperCase();
    }

    public String getCertCache() {
        return DigestUtils.sha256Hex(cert).toUpperCase();
    }
}

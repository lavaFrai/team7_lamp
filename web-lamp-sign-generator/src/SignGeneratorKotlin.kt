import org.apache.commons.codec.digest.DigestUtils


class SignGeneratorKotlin(cert: String) {
    private var _cert: String? = cert

    fun getCertCache(): String {
        return DigestUtils.sha256Hex(_cert).uppercase()
    }

    fun getPrivateKey(uid: Int): String {
        val keyData = StringBuilder()
        keyData.append("<")
        keyData.append(uid)
        keyData.append(">:<")
        keyData.append(_cert)
        keyData.append(">")

        return DigestUtils.sha256Hex(keyData.toString()).uppercase()
    }
}

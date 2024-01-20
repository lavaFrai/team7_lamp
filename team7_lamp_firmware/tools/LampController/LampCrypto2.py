from .LampCryptoModule import LampCryptoModule


class LampCrypto2:
    def __init__(self, cert):
        self.cert = cert
        self.crypro_module = LampCryptoModule(cert)

    def decrypt_package(self, package):
        return self.crypro_module.decrypt_data_package(package)

    def encrypt_package(self, data, uid):
        return self.crypro_module.encrypt_data_to_package(data, uid)

    def lamp_color_package(self, hex_color, uid):
        return self.encrypt_package("010C" + hex_color, uid)

    def lamp_brightness_package(self, hex_brightness, uid):
        return self.encrypt_package("010B" + hex_brightness, uid)


if __name__ == "__main__":
    crypto = LampCrypto2("g5jsb8be6xbek066ertm76bw0xeztlja5b38hdke")
    # print(crypto.lamp_brightness_package("10", 228))
    # print(crypto.lamp_color_package("FFFFFF", 228))

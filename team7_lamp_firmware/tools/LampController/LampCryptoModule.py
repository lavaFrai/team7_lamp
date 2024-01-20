import binascii
import json
import random
from hashlib import sha256
from .Cipher import Cipher


class LampCryptoModule:
    class PackageDecodingError(Exception):
        pass

    def __init__(self, cert):
        self.cert = cert

    def gen_private_key(self, uid):
        return sha256(f"<{uid}>:<{self.cert}>".encode()).digest().hex().upper()

    def gen_data_signature(self, data, uid, salt):
        return sha256(f"<{data}>:<{uid}>:<{self.gen_private_key(uid)}>:<{salt}>".encode()).digest().hex().upper()

    def verify_data_signature(self, data, uid, salt, signature):
        # print(self.gen_data_signature(data, uid, salt))
        # print(signature)
        try:
            return binascii.unhexlify(self.gen_data_signature(data, uid, salt)) == binascii.unhexlify(signature)
        except binascii.Error:
            return False

    def encrypt_data_to_package(self, data, uid):
        package = {}

        if type(data) != bytes or type(data) != bytearray:
            data = data.encode(encoding="utf8")

        private_key = self.gen_private_key(uid)
        salt = random.randint(0, 2 ** 15)
        cipher = Cipher(private_key.encode(), salt)

        package["data"] = binascii.hexlify(cipher.encrypt_block(data)).decode()
        package["salt"] = salt
        package["uid"] = uid
        package["signature"] = self.gen_data_signature(data.decode(), uid, salt).lower()

        return json.dumps(package)

    def decrypt_data_package(self, package):
        lp = LampPackage(package, self)
        if not lp.verify_signature():
            raise self.PackageDecodingError()
        return lp.data


class LampPackage:
    def __init__(self, package, crypto_module):
        self.crypto_module = crypto_module
        self.package = package
        self.json = json.loads(package)

        self.uid = self.json["uid"]
        self.salt = self.json["salt"]
        self.encrypted_data = self.json["data"]
        self.signature = self.json["signature"]

        cipher = Cipher(self.crypto_module.gen_private_key(self.uid).encode(), self.salt)
        try:
            self.data = cipher.decrypt_hex_block(self.encrypted_data).decode()
        except (UnicodeDecodeError, binascii.Error):
            self.data = None

    def verify_signature(self):
        return self.crypto_module.verify_data_signature(self.data, self.uid, self.salt, self.signature)

    def get_decrypted_data(self):
        return self.data


if __name__ == "__main__":
    package = '{"data": "06b1ade48387efe49635624d", "salt": 12878, "uid": 228, "signature": "96c05833713d0ed5a67613117f4d410180f126a1f20a97f90f5ac0ea21084b57"}'

    crypto_module = LampCryptoModule("g5jsb8be6xbek066ertm76bw0xeztlja5b38hdke")

    d = crypto_module.encrypt_data_to_package("Holla u too!", 228)
    # print(crypto_module.decrypt_data_package(d))
    # print(crypto_module.encrypt_data_to_package("010CFFFFFF", 228))
    print(crypto_module.gen_private_key(228))

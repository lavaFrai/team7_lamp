import binascii
from hashlib import sha256


class Cipher:
    def __init__(self, key, salt):
        self.key = key
        self.salt = salt

        self.gamma = None
        self.gamma_size = 32
        self.gamma_offset = 0
        self.round = 0
        self.generate_next_gamma()

    def reset(self):
        self.gamma_offset = 0
        self.round = 0
        self.generate_next_gamma()

    def next_byte(self, byte):
        if self.gamma_offset >= self.gamma_size:
            self.generate_next_gamma()
        res = byte ^ self.gamma[self.gamma_offset]
        # print(byte, self.gamma[self.gamma_offset], byte ^ self.gamma[self.gamma_offset])
        self.gamma_offset += 1
        return res

    def generate_next_gamma(self):
        self.round += 1
        self.gamma_offset = 0
        # print(self.key + b":" + str(self.round).encode() + b":" + str(self.salt).encode())
        self.gamma = sha256(self.key + b":" + str(self.round).encode() + b":" + str(self.salt).encode()).digest()
        # print(self.gamma.hex())

    def encrypt_block(self, data):
        return bytes(self.next_byte(byte) for byte in data)

    def decrypt_hex_block(self, data):
        return self.decrypt_block(binascii.unhexlify(data))

    def decrypt_block(self, data):
        return self.encrypt_block(data)

# 3FEBBBD74312B38224A5CE346144B75176AD41F4736DA0D07854549EA17BE0F9
# 3febbbd74312b38224a5ce346144b75176ad41f4736d0ad07854549ea17b0ef9

if __name__ == "__main__":
    key = b"465E29365E646F87D6E860EC2A1E9CEEEF47FB5D762F076D21CA7A32581D0F02"

    # text = b"Hello World"
    encrypted = b"a390e43430c80930cb5207"
    salt = 222025423

    # cipher = Cipher(key, salt)
    # print(text.decode(), "->", cipher.encrypt_block(text).hex())
    cipher = Cipher(key, salt)
    print(encrypted.decode(), "->", cipher.decrypt_hex_block(encrypted).decode())

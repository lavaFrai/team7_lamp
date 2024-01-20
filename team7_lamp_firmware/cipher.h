#include <EspCrypto.h>
#define CIPHER_KEY_SIZE (SHA256_SIZE * 2)
#define CIPHER_GAMMA_SIZE SHA256_SIZE


class Cipher {
public:
    Cipher(char* key, uint32 salt) {
        _salt = salt;
        _key = key;
        generateNextGamma();
    }

    byte nextByte(byte data) {
        if (currentGammaOffset >= CIPHER_GAMMA_SIZE) generateNextGamma();

        //Serial.print(data, DEC);
        //Serial.print("^");
        //Serial.print(_currentGamma[currentGammaOffset], HEX);
        //Serial.print(" ");
        //Serial.print(data ^ _currentGamma[currentGammaOffset], DEC);
        //Serial.println(" ");

        return data ^ _currentGamma[currentGammaOffset++];
    }

private:
    void generateNextGamma() {
        round++;
        currentGammaOffset = 0;
        SHA256 hasher;

        String roundId = String();
        roundId += _key;
        roundId += ":";
        roundId += round;
        roundId += ":";
        roundId += _salt;

        //Serial.println();
        //Serial.println(roundId);

        hasher.doUpdate(roundId.c_str());
        hasher.doFinal((byte*)_currentGamma);

        //Serial.println();
        //for (int i = 0; i < 32; i++) {
        //    Serial.print(_currentGamma[i], HEX);
        //    Serial.print(" ");
        //}
        //Serial.println();
    }

    uint32 round = 0;
    uint32 currentGammaOffset = 0;
    uint32 _salt;

    char* _key;
    char _currentGamma[CIPHER_GAMMA_SIZE];
};
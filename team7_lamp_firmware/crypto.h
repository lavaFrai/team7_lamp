#pragma once

#include <EspCrypto.h>
#include "log.h"
#include "consts.h"
#include "cipher.h"
#include "binascii.h"
#include <ArduinoJson.h>


RNG crypto_random_generator;

String crypto_generate_encryted_package_from_data(String data);
String crypto_generate_data_from_encryted_package(String data);
String crypto_generate_encryted_package_from_data(const char* data);


void crypto_init() {
    char plain_text[] = "Hello World";
    
    print_timestamp();
    Serial.print("Test encrypting package:\t");

    String enc = crypto_generate_encryted_package_from_data("Hello World!");
    
    Serial.println(
        enc
    );

    
    print_timestamp();
    Serial.print("Test decrypting package:\t");
    String dec = crypto_generate_data_from_encryted_package(enc);
    Serial.println(
        dec
    );
    print_timestamp();
    Serial.print("Test decrypting package:\t");
    dec = crypto_generate_data_from_encryted_package(enc);
    Serial.println(
        dec
    );
}


String crypto_calculate_data_hash(String data, uint32 salt) {
    String hash = String();
    SHA256 crypto_sha256_hasher;

    hash = "<" + data + ">:<" + UID + ">:<" + PRIVATE_KEY + ">:<" + salt + ">";
    //<сообщение, которое мы отправили>:<uid лампы>:<приватный ключ>:<рандомное число, чтобы коды для одинаковых сообщений не совпадали>

    byte hash_bytes[SHA256_SIZE]; 
    crypto_sha256_hasher.doUpdate(hash.c_str());
    crypto_sha256_hasher.doFinal(hash_bytes);

    hash = "";
    for (int i = 0; i < SHA256_SIZE; i++) {
        hash += hexify_byte(hash_bytes[i]);
    }

    return hash;
}

String crypto_encrypt_hex_block(String data, uint32 salt) {
    Cipher cipher(PRIVATE_KEY, salt);
    String data_e;
    for (int i = 0; i < data.length(); i++) {
        data_e += hexify_byte(cipher.nextByte(data[i]));
    }
    return data_e;
}

String crypto_decrypt_hex_block(String data, uint32 salt) {
    Cipher cipher(PRIVATE_KEY, salt);
    String data_e;

    for (int i = 0; i < data.length() / 2; i++) {
        data_e += (char)cipher.nextByte(unhexify_byte(data, i*2));
        // data_e += String(unhexify_byte(data, i*2), HEX);
    }

    return data_e;
}

String crypto_generate_encryted_package_from_data(String data) {
    String package = String();
    uint32 salt = crypto_random_generator.getLong();

    DynamicJsonDocument package_j(2048);

    data.replace("\\\"", "\"");

    package_j["data"] = crypto_encrypt_hex_block(data, salt);
    /*
    package_j["data_d"] = crypto_decrypt_hex_block(
        // String("686578"),
        crypto_encrypt_hex_block(data, salt), 
        salt);
        */
    package_j["salt"] = salt;
    package_j["uid"] = UID;
    package_j["signature"] = crypto_calculate_data_hash(data, salt);

    serializeJson(package_j, package);

    return package;
}

String crypto_generate_encryted_package_from_data(const char* data) {
    return crypto_generate_encryted_package_from_data(String(data));
}

String crypto_generate_data_from_encryted_package(String data) {
    String package;
    DynamicJsonDocument package_j(2048);
    DeserializationError error = deserializeJson(package_j, data);

    if (error != DeserializationError::Ok) { package = "!ERR::DECODING_ERROR";return package; }

    uint32 salt = package_j["salt"];
    int uid = package_j["uid"];
    String signature = package_j["signature"];
    String data_e = package_j["data"];

    if (UID != uid) { package = "!ERR::INVALID_UID";return package; }
    String data_d = crypto_decrypt_hex_block(data_e, salt);

    String hash = crypto_calculate_data_hash(data_d, salt);
    if (signature != hash) { package = "!ERR::INVALID_SIGN:"+data_d+":"+String(salt)+":"+hash+":"+signature;return package; }

    return data_d;
}

String get_random_hex_bytes(int count) {
    String res;
    for (int i = 0; i < count; i++) {
        res += hexify_byte(crypto_random_generator.get());
    }
    return res;
}
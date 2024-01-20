#pragma once
#include <ESP8266WiFi.h>
#include <EspCrypto.h>
#include <PubSubClient.h>
#include "log.h"
#include "led.h"
#include "binascii.h"
#include "crypto.h"


char* MQTT_HOST =     "lavafrai.ml";
int   MQTT_PORT =       1883;
char* MQTT_LOGIN =    "smarthomerussia";
char* MQTT_PASSWORD = "smarthomerussia";
char* MQTT_TOPIC =    "topic.lamp.{{UID}}";

void callback(char* topic, byte* payload, unsigned int length);

#define MQTT_BUFFER_SIZE (50)
char mqtt_buffer[MQTT_BUFFER_SIZE];

WiFiClient mqtt_wifi_client;
PubSubClient mqtt_client(mqtt_wifi_client);

void mqtt_init() {
    print_timestamp();
    Serial.println("initialising MQTT");

    mqtt_client.setServer(MQTT_HOST, MQTT_PORT);
    mqtt_client.setCallback(callback);
}

void mqtt_loop() {
    if (!mqtt_client.connected()) {
        print_timestamp();
        Serial.println("Reconnecting MQTT");

        String random_bytes = get_random_hex_bytes(16);
        String _mqtt_uid = "lamp." + String(UID) + "." + random_bytes;

        if (mqtt_client.connect(_mqtt_uid.c_str(), MQTT_LOGIN, MQTT_PASSWORD)) {
            print_timestamp();
            Serial.print("MQTT connected as ");
            Serial.println(_mqtt_uid);

            // mqtt_client.publish("outTopic", "hello world");
            String _topic = MQTT_TOPIC;
            _topic.replace("{{UID}}", String(UID));
            mqtt_client.subscribe(_topic.c_str());
            print_timestamp();
            Serial.print("MQTT subscribing topic ");
            Serial.println(_topic);
        } else {
            print_timestamp();
            Serial.println("Connection failed");
        }
    } else {
        mqtt_client.loop();
    }
}

void broadcastHandler(byte* payload, unsigned int length) {
    led_set_color(
        unhexify_int(
            payload, length
        )
    );
}

void callback(char* topic, byte* payload, unsigned int length) {
    print_timestamp();

    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    // for (int i = 0; i < length; i++) {
    //     Serial.print((char)payload[i]);
    // }
    // Serial.println();

    // Converting payload to string
    String data_e;
    for (int i = 0; i < length; i++) {
        data_e += (char)payload[i];
    }

    // End converting payload to string

    String data = crypto_generate_data_from_encryted_package(data_e);
    Serial.print(data_e);
    Serial.println();

    if (data != "" && data[0] != '!') {
        // print_timestamp();
        // Serial.println(data);

        // On data verified and decrypted
        on_mqtt_command(data);
        // End on data verified and decrypted
    }

    // broadcastHandler(payload, length);
}
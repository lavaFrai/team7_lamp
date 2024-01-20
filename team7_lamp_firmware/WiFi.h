#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "log.h"
#include "events.h"
#include "wifi_manager.h"

int last_status = WL_CONNECTED;


/*
char* WIFI_SSID =      "dendr";
char* WIFI_PASSWORD =  "35279601";
*/
char* WIFI_SSID =      "Qqqqq";
char* WIFI_PASSWORD =  "s8a497bj";

WiFiManager wm("WebLamp_wifi_setup");


void wifi_init() {
    print_timestamp();
    Serial.print("Connecting ");
    Serial.print(WIFI_SSID);
    Serial.println("...");

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    on_wifi_connected();
    print_timestamp();
    Serial.println("Wifi connected");
}

void wifi_init_test() {
    WiFi.setHostname("Web Lamp");
    wm.autoConnect();
}

void wifi_loop() {
    // print_timestamp();
    // Serial.println("WiFi loop");

    wl_status_t wifi_status = WiFi.status();
    if (WiFi.status() != WL_CONNECTED) {
        print_timestamp();
        Serial.print("Reconnecting ");
        Serial.print(WIFI_SSID);
        Serial.println("...");

        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

        if (WiFi.waitForConnectResult() == WL_CONNECTED) {
            print_timestamp();
            Serial.println("Wifi connected");
            on_wifi_connected();
        } else if (last_status == WL_CONNECTED && wifi_status != WL_CONNECTED) {
            print_timestamp();
            Serial.println("Connection disconnected");
            on_wifi_disconnected();
        }

    }

    last_status = wifi_status;
}

void wifi_loop_test() {
    wl_status_t wifi_status = WiFi.status();
    if (WiFi.status() != WL_CONNECTED) {
        print_timestamp();
        Serial.print("Reconnecting ");
        Serial.print(wm.stored_ssid);
        Serial.println("...");

        WiFi.mode(WIFI_STA);
        WiFi.begin(wm.stored_ssid, wm.stored_password);

        if (wm.waitForConnectResult(5000) == WL_CONNECTED) {
            print_timestamp();
            Serial.println("Wifi connected");
            on_wifi_connected();
        } else if (last_status == WL_CONNECTED && wifi_status != WL_CONNECTED) {
            print_timestamp();
            Serial.println("Connection disconnected");
            on_wifi_disconnected();
        }

    }

    last_status = wifi_status;
}
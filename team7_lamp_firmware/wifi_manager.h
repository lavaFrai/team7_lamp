#pragma once

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "wifi_manager_pages.h"
#include <EEPROM.h>
#include "events.h"
#include "safe_delay.h"
#include "led.h"


const int wm_scan_interval = 10000;
const byte WIFI_MANAGER_DNS_PORT = 53;
IPAddress WIFI_MANAGER_AP_IP(192, 168, 1, 1);
DNSServer wm_dnsServer;
ESP8266WebServer wm_webServer(80);
int wm_networks_count = 0;
int wm_last_scan = -wm_scan_interval;

struct wm_settings {
    uint64 signature;
    uint16 version;
    uint16 ssid_size;
    uint16 password_size;
};

class WiFiManager {
    bool running = false;
    String wm_ap_name;
    uint8 current_version = 0;
    uint64 signature = 0x776D6C66;


public:
    String stored_password;
    String stored_ssid;

    WiFiManager(String ap_name) {
        wm_ap_name = ap_name;
    }

    WiFiManager(const char* ap_name) {
        wm_ap_name = String(ap_name);
        
    }

    void autoConnect() {
        init_eeprom();
        // store_eeprom_data(String("dendr"), String("35279601"));

        if (has_eeprom_data()) {
            print_timestamp();
            Serial.println("Connecting to saved wifi");

            load_eeprom_data();
            WiFi.mode(WIFI_STA);

            print_timestamp();
            Serial.print("Connecting ");
            Serial.print("SSID: ");
            Serial.print(stored_ssid);
            Serial.print("\t Password: ");
            Serial.println(stored_password);

            WiFi.begin(stored_ssid, stored_password);
            led_loop();

            if (WiFi.waitForConnectResult(25000) == WL_CONNECTED) {
                print_timestamp();
                Serial.println("Wifi connected");
                on_wifi_connected();
            } else {
                print_timestamp();
                Serial.println("Connection failed");
                run();
            }
        } else {
            run();
        }
    }

    int8 waitForConnectResult(unsigned long timeoutLength) {
        //1 and 3 have STA enabled

        uint32 timer = millis();

        while(millis() - timer < timeoutLength) {
            if(WiFi.status() == WL_CONNECTED) {
                return WiFi.status();
            }
            led_loop();
            delay(0);
            ESP.wdtFeed();
        }
        return -1; // -1 indicates timeout
    }

    void run() {
        running = true;

        print_timestamp();
        Serial.println("Running web WiFi manager");

        on_wifi_manager_started();

        wm_setup();
        while (running) {
            wm_loop();
            led_loop();
        }
    }

    void wm_setup() {
        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(WIFI_MANAGER_AP_IP, WIFI_MANAGER_AP_IP, IPAddress(255, 255, 255, 0));
        WiFi.softAP(wm_ap_name);

        
        // store_eeprom_data(String("dendr"), String("35279601"));

        wm_dnsServer.start(WIFI_MANAGER_DNS_PORT, "*", WIFI_MANAGER_AP_IP);

        wm_webServer.onNotFound([]() { wm_webServer.sendHeader("Location", "http://weblamp.local/"); wm_webServer.send(302, "text/plane", ""); });
        wm_webServer.on("/", std::bind(&WiFiManager::wm_handle_root, this));
        wm_webServer.on("/wifi", std::bind(&WiFiManager::wm_handle_wifi, this));
        wm_webServer.on("/connect", std::bind(&WiFiManager::wm_handle_connect, this));
        wm_webServer.on("/connect-confirm", std::bind(&WiFiManager::wm_handle_connect_confirm, this));
        wm_webServer.on("/style.css", []() { wm_webServer.send(200, "text/css", wm_stylePage); });
        wm_webServer.begin();
    }

    void wm_loop() {
        if (millis() - wm_last_scan > wm_scan_interval) {
            wm_networks_count = WiFi.scanNetworks();
            wm_last_scan = millis();
        }

        wm_dnsServer.processNextRequest();
        wm_webServer.handleClient();
        led_loop();
    }

    void wm_handle_connect_confirm() {
        String message = wm_confirmPage;
        message += wm_webServer.args();

        String ssid = "";
        String password = "";

        for (int i = 0; i < wm_webServer.args(); i++) 
        {
            if (wm_webServer.argName(i) == "password") password = wm_webServer.arg(i);
            if (wm_webServer.argName(i) == "ssid") ssid = wm_webServer.arg(i);
        } 

        message.replace("{{ssid}}", ssid);
        message.replace("{{password-safe}}", (password == "" ? "(Empty)" : password));
        message.replace("{{password}}", password);

        if (ssid == "") {
            wm_webServer.sendHeader("Location", "http://weblamp.local/wifi");
            wm_webServer.send(302, "text/plane", "");
        }



        wm_webServer.send(200, "text/html", message);
    }

    void wm_handle_connect() {
        String message = wm_connectPage;

        String ssid = "";
        String password = "";

        for (int i = 0; i < wm_webServer.args(); i++) 
        {
            if (wm_webServer.argName(i) == "password") password = wm_webServer.arg(i);
            if (wm_webServer.argName(i) == "ssid") ssid = wm_webServer.arg(i);
        } 

        message.replace("{{ssid}}", ssid);
        message.replace("{{password-safe}}", (password == "" ? "(Empty)" : password));
        message.replace("{{password}}", password);
        message.replace("{{integred-styles}}", wm_stylePage);

        wm_webServer.send(200, "text/html", message);
        store_eeprom_data(ssid, password);

        on_wifi_disconnected();
        safe_delay(3000);
        WiFi.softAPdisconnect (true);
        safe_delay(1000);
        ESP.restart();
    }

    void wm_handle_wifi() {
        // <li><a class=\"wifiname\">smartweb</a>\t<span class=\"unfocused-text\">5%</span> L</li>
        int networks[wm_networks_count];

        for (int i = 0; i < wm_networks_count; i++) {
            networks[i] = i;
        }

        for (int i = 0; i < wm_networks_count; i++) {
            for (int j = i + 1; j < wm_networks_count; j++) {
                if (WiFi.RSSI(networks[j]) > WiFi.RSSI(networks[i])) {
                    std::swap(networks[i], networks[j]);
                }
            }
        }

        String wifilist = "";
        for (int i = 0; i < wm_networks_count; i++) {
            wifilist += "<li><a class=\"wifiname\">" + WiFi.SSID(networks[i]) + "</a>\t<span class=\"unfocused-text\">" + String(rssi_to_percents(WiFi.RSSI(networks[i]))) + "%</span> " + ((WiFi.encryptionType(networks[i]) == ENC_TYPE_NONE)?"":"L") + "</li>\n";
        }

        String page = wm_wifiPage;
        page.replace("{{wifilist}}", wifilist);

        wm_webServer.send(200, "text/html", page);
    }

    void wm_handle_root() {
        if (wm_webServer.hostHeader() != "weblamp.local") {
            wm_webServer.sendHeader("Location", "http://weblamp.local/");
            wm_webServer.send(302, "text/plane", "");
        }

        wm_webServer.send(200, "text/html", wm_rootPage);
    }

    int rssi_to_percents(int RSSI) {
        int quality = 0;

        if (RSSI <= -100) {
            quality = 0;
        } else if (RSSI >= -50) {
            quality = 100;
        } else {
            quality = 2 * (RSSI + 100);
        }
        return quality;
    }

    void init_eeprom() {
        EEPROM.begin(128);
    }

    bool has_eeprom_data() {
        wm_settings settings;
        EEPROM.get(0, settings);

        return settings.signature == signature && settings.version == current_version;
    }

    void load_eeprom_data() {
        wm_settings settings;
        String ssid;
        String password;
        EEPROM.get(0, settings);

        for (int i = 0; i < settings.ssid_size; i++) {
            ssid += (char)EEPROM.read(sizeof(settings) + i);
        }
        for (int i = 0; i < settings.password_size; i++) {
            password += (char)EEPROM.read(sizeof(settings) + ssid.length() + i);
        }
        
        print_timestamp();
        Serial.println("Loading WM settings from eeprom");
        print_timestamp();
        Serial.print("Sign verify: ");
        Serial.println(has_eeprom_data() ? "true" : "false");
        print_timestamp();
        Serial.println("SSID: " + ssid);
        print_timestamp();
        Serial.println("Password: " + password);

        stored_password = password;
        stored_ssid = ssid;
    }

    void store_eeprom_data(String ssid, String password) {
        wm_settings settings;
        settings.signature = signature;
        settings.version = current_version;
        settings.ssid_size = ssid.length();
        settings.password_size = password.length();

        print_timestamp();
        Serial.print("Storing: ");
        Serial.print(ssid);
        Serial.print(" : ");
        Serial.println(password);

        EEPROM.put(0, settings);

        for (int i = 0; i < ssid.length(); i++) {
            EEPROM.write(sizeof(settings) + i, ssid[i]);
        }
        for (int i = 0; i < password.length(); i++) {
            EEPROM.write(sizeof(settings) + ssid.length() + i, password[i]);
        }

        EEPROM.commit();
    }
};
#pragma once
#include <Arduino.h>
#include "consts.h"


void print_timestamp() {
    long m = millis();
    String mls = String(m % 1000);
    while (mls.length() < 3) mls = "0" + mls;

    Serial.print("[" + String(m / 1000) + "." + mls + "]\t");
}

void serial_init() {
    Serial.begin(115200);
    delay(100);
    Serial.println();
    Serial.println();
    Serial.println();
    print_timestamp();
    Serial.println("Initializing lamp with uid 0x" + String(UID, HEX));
}
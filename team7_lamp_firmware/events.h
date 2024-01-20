#pragma once
#include "led.h"
#include "log.h"
#include "binascii.h"


void on_wifi_connected() {
    led_set_color(0xFFFFFF);
}

void on_wifi_disconnected() {
    led_set_color(0xFF2222);
}

void on_wifi_manager_started() {
    led_set_color(0x2222FF);
}

void on_mqtt_command(String command) {
    print_timestamp();
    Serial.print("Handling command: ");
    Serial.println(command);

    if (true) {// ignoring version controll
        int instruction = command[3];

        switch (instruction)
        {
        case 'C':
            print_timestamp();
            Serial.print("Changing color");

            led_set_color(
                unhexify_int(
                    (byte*)(command.c_str() + 4), 6
                )
            );

            break;
        case 'B':
            print_timestamp();
            Serial.println("Changing brightness");

            led_set_brightness(
                unhexify_int(
                    (byte*)(command.c_str() + 4), 2
                )
            );
            break;
        case 'E':
            print_timestamp();
            Serial.println("Running effect");
            break;
        
        default:
            break;
        }
    }
}
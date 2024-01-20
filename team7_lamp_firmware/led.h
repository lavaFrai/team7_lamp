#pragma once
#include <FastLED.h>

const int COUNT_OF_LEDS = 9;
const int LEDS_DATA_PIN = 0;
uint64 last_color_upgate = 0;

CRGB leds[COUNT_OF_LEDS];
int current_color = (0xFF2222);
int target_color = (0xFF2222);

uint8 current_brightness = 100;
uint8 target_brightness = 100;


void led_init() {
    FastLED.addLeds<WS2812B, LEDS_DATA_PIN, GRB>(leds, COUNT_OF_LEDS);
    FastLED.setBrightness(100);
    FastLED.showColor(CRGB(current_color));
}

void led_loop() {
    if (millis() - last_color_upgate > 10) {
        int current_color_r = current_color % 0x1000000 / 0x10000;
        int current_color_g = current_color % 0x10000   / 0x100;
        int current_color_b = current_color % 0x100;
        
        int target_color_r = target_color % 0x1000000 / 0x10000;
        int target_color_g = target_color % 0x10000   / 0x100;
        int target_color_b = target_color % 0x100;

        if (target_color_r > current_color_r) current_color_r+=1;
        if (target_color_r < current_color_r) current_color_r-=1;

        if (target_color_g > current_color_g) current_color_g+=1;
        if (target_color_g < current_color_g) current_color_g-=1;

        if (target_color_b > current_color_b) current_color_b+=1;
        if (target_color_b < current_color_b) current_color_b-=1;
        current_color = (current_color_r << 16) | (current_color_g << 8) | (current_color_b);

        
        if (target_brightness > current_brightness) current_brightness+=1;
        if (target_brightness < current_brightness) current_brightness-=1;


        // print_timestamp();
        // Serial.print(target_color, HEX);
        // Serial.print(" ");
        // Serial.println(current_color, HEX);

        last_color_upgate = millis();


        FastLED.setBrightness(current_brightness);
        FastLED.showColor(CRGB(current_color));
    }
};

void led_set_color(uint32 color) {
    print_timestamp();

    Serial.print("Setting color ");
    Serial.println(color, HEX);

    target_color = color;
}

void led_set_brightness(uint8 brightness) {
    print_timestamp();

    Serial.print("Setting brightness ");
    Serial.println(brightness, HEX);

    target_brightness = brightness;
}

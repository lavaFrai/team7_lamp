#pragma once
#include "led.h"


void safe_delay(unsigned long ms)
{
    while (ms > 0) {
        delay(5);
        ms -= 5;
        led_loop();
    }
}
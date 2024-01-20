#include <ESP8266WiFi.h>
#include <Ticker.h>

#include "Log.h"
#include "WiFi.h"
#include "MQTT.h"
#include "consts.h"
#include "crypto.h"
#include "led.h"
#include "safe_delay.h"

void ICACHE_RAM_ATTR loop_fixed();


void setup() {
  serial_init();
  led_init();
  led_loop();
  crypto_init();
  
  safe_delay(2000);

  
  // timer1_attachInterrupt(loop_fixed);
  // timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  // timer1_write(6000);

  wifi_init_test();

  mqtt_init();

  print_timestamp();
  Serial.println("Initialisation finished");

  // Serial.println(unhexify_int("ACFF12"), HEX);
}

void loop() {
  wifi_loop_test();

  // когда WiFi подключен
  if (WiFi.status() == WL_CONNECTED) {
    mqtt_loop();
  }

  led_loop();
}

void ICACHE_RAM_ATTR loop_fixed() {
  led_loop();
  // timer1_write(2000);
}

//void yield() {
//  led_loop();
//  ESP.wdtFeed();
//}

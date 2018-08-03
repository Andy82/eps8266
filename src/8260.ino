#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <TickerScheduler.h>         //https://github.com/Toshik/TickerScheduler

const bool DEBUG = true;


// Web интерфейс для устройства
ESP8266WebServer HTTP(80);
TickerScheduler ts(3);


void setup() {

  Log_init();
  Hardware_init();
  Time_init();
  FS_init();
  Config_init();
  Cmd_init();
  WIFI_init();
  Update_init();
  HTTP_init();
  SSDP_init();
  //Telegram_init();
  MQTT_init();
  DDNS_init();
  Controller();
  log("Init compleated");
}

void loop() {
  ts.update();
  Cmd_loop();
  Serial_loop();
  HTTP_loop();
  MQTT_loop();
}

#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <update.h>

// Объект для обнавления с web страницы
ESP8266HTTPUpdateServer httpUpdater;

void Update_init() {
  // Добавляем функцию Update для перезаписи прошивки по Wi-Fi при 1М(256K SPIFFS) и выше
  httpUpdater.setup(&HTTP);
  MDNS.addService("http", "tcp", 80);
}
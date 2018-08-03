#include <ESP8266HTTPUpdateServer.h> //Содержится в пакете
#include <ESP8266httpUpdate.h>       //Содержится в пакете

// Объект для обнавления с web страницы
ESP8266HTTPUpdateServer httpUpdater;

void Update_init() {
  // Добавляем функцию Update для перезаписи прошивки по Wi-Fi при 1М(256K SPIFFS) и выше
  httpUpdater.setup(&HTTP);
}


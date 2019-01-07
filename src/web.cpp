#include <web.h>

// Web интерфейс для устройства

void HTTP_init(void) {
  // Кэшировать файлы для быстрой работы
  //HTTP.serveStatic("/css/", SPIFFS, "/css/", "max-age=31536000"); // кеширование на 1 год
  //HTTP.serveStatic("/js/", SPIFFS, "/js/", "max-age=31536000"); // кеширование на 1 год
  //HTTP.serveStatic("/img/", SPIFFS, "/img/", "max-age=31536000"); // кеширование на 1 год

  // Запускаем HTTP сервер
  HTTP.begin();
  logger.log("HTTP Server run.");
}

void HTTP_loop() {
  HTTP.handleClient();
}

void HttpCallback(String command) { 
  HTTP.send(200, "text/plain", command);
}
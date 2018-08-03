#include <ESP8266HTTPClient.h>

// --------------------- Включаем DDNS
ESP8266WebServer HTTPWAN;
String ddns;
String ddnsName;
int ddnsPort;

void DDNS_init() {
  ddns      = jConfig.jRead("ddns");
  ddnsName  = jConfig.jRead("ddnsName");
  ddnsPort  = jConfig.jRead("ddnsPort").toInt();

  ESP8266WebServer HTTPWAN(ddnsPort);

  HTTPWAN.begin();
    // задача синхронизайия с сервером ddns каждые 5 минут
 ts.add(1, 300000, [&](void*) {
    ip_wan(ddns);
  }, nullptr, true);
}

// ------------------------------Установка параметров ddns
void handle_ddns() {
  ddns      = HTTP.arg("ddns");
  ddnsName  = HTTP.arg("ddnsName");
  ddnsPort  = HTTP.arg("ddnsPort").toInt();

  jConfig.jWrite("ddns",      ddns);
  jConfig.jWrite("ddnsName",  ddnsName);
  jConfig.jWrite("ddnsPort",  ddnsPort);
  ip_wan(jConfig.jRead("ddns"));
  saveConfig();
  HTTP.send(200, "text/plain", "OK");
}

// --------------------------------Запрос для синхронизации внешнего ip адреса с ddns
int ip_wan(String ddns) {
  if (ddns != "") {
    log("Update DDNS");
    getURL(ddns);
  }
}

// Запрос на удаленный URL
String getURL(String urls) {
  String answer = "";
  HTTPClient http;
  http.begin(urls); //HTTP
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    answer = http.getString();
  }
  http.end();
  return answer;
}
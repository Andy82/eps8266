#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ddns.h>

// Включаем DDNS
ESP8266WebServer HTTPWAN;
String ddns;
String ddnsName;
int ddnsPort;

unsigned long startTimeDDNS = millis();

void DDNS_init() {
  ddns      = jConfig.jRead("ddns");
  ddnsName  = jConfig.jRead("ddnsName");
  ddnsPort  = jConfig.jRead("ddnsPort").toInt();

  if (ddns != "" &&  jConfig.jRead("WifiMode") == "STA") {
    ESP8266WebServer HTTPWAN(ddnsPort);
    HTTPWAN.begin();
    logger.log("DDNS service started.");
  }
}

void DDNS_loop()
{
  if (millis() - startTimeDDNS > 300000) { // run every 5 minutes
    startTimeDDNS = millis();
    ip_wan();
  }
}


//Запрос для синхронизации внешнего ip адреса с ddns
void ip_wan() {
  ddns = jConfig.jRead("ddns");
  if (ddns != "") {
    logger.log("Update DDNS");
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
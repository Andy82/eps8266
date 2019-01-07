#include <ESP8266WiFi.h>
#include <wifi.h>


String hostName;
String ssidName;
String ssidPassword;
String ssidAPName;
String ssidAPPassword;
String predefinedIp;
String ip;
String getway;
String subnet;
int tries;
int blink;

// Инициализация WiFi
void WIFI_init() {
  logger.log("Wifi configuration read");
  hostName      = jConfig.jRead("chipID");
  ssidName      = jConfig.jRead("ssidName");
  ssidPassword  = jConfig.jRead("ssidPassword");
  ssidAPName    = jConfig.jRead("ssidAPName");
  ssidAPPassword= jConfig.jRead("ssidAPPassword");
  predefinedIp  = jConfig.jRead("predefinedIp");
  ip            = jConfig.jRead("ip");
  getway        = jConfig.jRead("getway");
  subnet        = jConfig.jRead("subnet");
  tries         = jConfig.jRead("tries").toInt();
  blink         = jConfig.jRead("blink").toInt();

  if (startSTA()) {
      logger.log("Wifi running as STA");
      jConfig.jWrite("WifiMode", "STA"); 
  }
  else {
    startAP();
    logger.log("Wifi running as AP");
    jConfig.jWrite("WifiMode", "AP"); 
  }

    jConfig.jWrite("ip", ip);
    jConfig.jWrite("getway", getway);
    jConfig.jWrite("subnet", subnet);

    logger.log("IP = " + ip);
    logger.log("Getway = " + getway);
    logger.log("Subnet = " + subnet);

    jConfig.jWrite("WifiStatus", WiFi.status()); 
}

//Подключение к роутеру конфигурация в строке json
boolean startSTA() {
  WiFi.persistent (false);
  if (predefinedIp == "1") {
    IPAddress staticIP;
    IPAddress staticGateway;
    IPAddress staticSubnet;

    boolean check = true;
    if (ip != "") {
      staticIP.fromString(ip);
      check = false;
    }
    if (getway != "") {
      staticGateway.fromString(getway);
      check = false;
    }
    if (subnet != "") {
      staticSubnet.fromString(subnet);
      check = false;
    }
    if (!check) {
      WiFi.config (staticIP, staticGateway, staticSubnet);
    }
  }
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  WiFi.hostname (hostName);
  WiFi.begin(ssidName.c_str(),ssidPassword.c_str());
  logger.log("WiFi initialized.");

  if (wifiConnect()) {
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    
    ip = WiFi.localIP().toString();
    getway =  WiFi.gatewayIP().toString();
    subnet = WiFi.subnetMask().toString();
    return true;
  }
  return false;
}

// Проверять на подключение к роутеру  tries раз. Отображать подключение на ноге pin
boolean wifiConnect() {
  if (tries == 0) tries = 11;
  while (--tries && WiFi.status() != WL_CONNECTED)
  {
    //Мигаем сетодиодом при попытке подключится к роутеру
    logger.logFlash(1,500);
  }
  if (WiFi.status() == WL_CONNECTED) return true;
  return false;
}


//Включение режима AP конфигурация в строке json
//{"ssidAP":"Sonoff","ssidApPass":""}
boolean startAP() {
  IPAddress apIP; 
  IPAddress staticGateway;
  IPAddress staticSubnet;

  apIP.fromString(jConfig.jRead("ipAP"));
  staticGateway.fromString(jConfig.jRead("getwayAP"));
  staticSubnet.fromString(jConfig.jRead("subnetAP"));

  // Задаем настройки сети
  WiFi.softAPConfig(apIP, staticGateway, staticSubnet);
  WiFi.softAP(ssidAPName.c_str(), ssidAPPassword.c_str());
  WiFi.mode(WIFI_AP);
  //Зажигаем светодиод если находимся в режиме AP
  return true;
}
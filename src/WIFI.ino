#include <ESP8266WiFi.h>

String jsonWifiNetworks = "";
String ssidName;
String ssidPassword;
String checkboxIP;
String ssidAPName;
String ssidAPPassword;
String ip;
String getway;
String subnet;
int tries;
int blink;

// Инициализация WiFi
void WIFI_init() {
  ssidName      = jConfig.jRead("ssidName");
  ssidPassword  = jConfig.jRead("ssidPassword");
  checkboxIP    = jConfig.jRead( "checkboxIP");
  ssidAPName    = jConfig.jRead("ssidAPName");
  ssidAPPassword= jConfig.jRead("ssidAPPassword");
  ip            = jConfig.jRead("ip");
  getway        = jConfig.jRead("getway");
  subnet        = jConfig.jRead("subnet");
  tries         = jConfig.jRead("tries").toInt();
  blink         = jConfig.jRead("blink").toInt();

  if (startSTA()) {
      log("Wifi running as STA");
  }
  else {
    startAP();
    log("Wifi running as AP");
  }
}


//-------------- Перизапустить wifi попытаться узнать будущий ip адрес перезагрузить устройство
bool handle_restartWiFi() {
  //Перезапуск Wi-Fi при первой настройке
  log("WiFi reconnect");
    WiFi.mode(WIFI_AP_STA);
  // Не отключаясь от точки доступа подключаемся к роутеру для получения будущего IP
  WiFi.begin(ssidName.c_str(), ssidPassword.c_str());

  wifiConnect(tries, blink);

  // {"title":"<h3>{{LangConnect2}} <a href="http://192.168.1.30">http://192.168.1.30</a></h3>"}
  // {"title":"Любой текст и html","class":"класс", "style":"Стиль","state":"Данные для вставки в state input_а"}
  String state = "\{\"title\":\"<h3> Перезагрузка <a href=http://";
  state = state + WiFi.localIP().toString();
  state = state + ">http://" + WiFi.localIP().toString() + "</a></h3>\" \\}";
  log("New IP: " + WiFi.localIP().toString());

  HTTP.send(200, "application/json", state);
  delay(2000);
  // Отключаем точку доступа и переподключаемся к роутеру
  ESP.restart();
}


//-------------Подключение к роутеру конфигурация в строке json
//{"ip":"192.168.0.190","getway":"192.168.0.1","subnet":"255.255.255.0","dns":"","checkboxIP":0}
boolean startSTA() {
  //WiFi.persistent (false);
  if (checkboxIP == "1") {
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
  WiFi.hostname (chipID);
  //WiFi.begin();
  WiFi.begin(ssidName.c_str(),ssidPassword.c_str());
  if ( wifiConnect(tries, blink)) {
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    ip = WiFi.localIP().toString();
    getway =  WiFi.gatewayIP().toString();
    subnet = WiFi.subnetMask().toString();
    jConfig.jWrite("ip", ip);
    jConfig.jWrite("getway", getway);
    jConfig.jWrite("subnet", subnet);
    return true;
  }
  return false;
}

// --------------Проверять на подключение к роутеру  tries раз. Отображать подключение на ноге pin
boolean wifiConnect(byte tries, byte pin) {
  if (tries == 0) tries = 11;
  if ((pin > 5 && pin < 12) || pin > 16) pin = 0 ;
  if (pin != 0)   pinMode(pin, OUTPUT);
  while (--tries && WiFi.status() != WL_CONNECTED)
  {
    //Мигаем сетодиодом при попытке подключится к роутеру
    //if (pin != 0)   digitalWrite(pin, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    //if (pin != 0)  digitalWrite(pin, LOW);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }
  if (WiFi.status() == WL_CONNECTED)return true;
  return false;
}


//-------------Включение режима AP конфигурация в строке json
//{"ssidAP":"Sonoff","ssidApPass":""}
boolean startAP() {
  IPAddress apIP(192, 168, 4, 1);
  IPAddress staticGateway(192, 168, 4, 1);
  IPAddress staticSubnet(255, 255, 255, 0);
  // Задаем настройки сети
  WiFi.softAPConfig(apIP, staticGateway, staticSubnet);
  // Включаем WIFI в режиме точки доступа с именем и паролем
  // хронящихся в переменных _ssidAP _passwordAP
  WiFi.softAP(ssidAPName.c_str(), ssidAPPassword.c_str());
  // Меняем режим на режим точки доступа
  WiFi.mode(WIFI_AP);
  //Зажигаем светодиод если находимся в режиме AP
  int pin = blink;
  if ((pin > 5 && pin < 12) || pin > 16) pin = 0 ;
  if (pin != 0)  {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }
  return true;
}


// ---------------- сканирование сети на доступные точки доступа
void handle_wifi_scan() {
  long lastScanMillis;
  long currentMillis = millis();

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  JsonArray& networks = json.createNestedArray("networks");

  // trigger Wi-Fi network scan
  if (currentMillis - lastScanMillis > 10000)
  {
    int n = WiFi.scanNetworks();
    Serial.print("\nScan start ... ");
    lastScanMillis = currentMillis;
    log(n + " network(s) found\n");
    for (int i = 0; i < n; i++) {
      JsonObject& data = networks.createNestedObject();
      data["ssid"] = WiFi.SSID(i);
      data["pass"] = (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "" : "*";
      data["dbm"] = WiFi.RSSI(i);
      Serial.printf("%d: %s, Ch:%d (%ddBm) %s\n", i+1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "");
    }
    jsonWifiNetworks = "";
    json.printTo(jsonWifiNetworks);
    WiFi.scanDelete();
  }
  HTTP.send(200, "application/json", jsonWifiNetworks);
}

// Функции API-Set
// Установка SSDP имени по запросу вида http://192.168.0.101/ssdp?ssdp=proba
void handle_ssdp() {
  jConfig.jWrite("SSDPName", HTTP.arg("ssdp")); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
  jConfig.jWrite("space", HTTP.arg("space"));
  saveConfig();                 // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
}
// Установка параметров для подключения к внешней AP по запросу вида http://192.168.0.101/ssid?ssid=home2&password=12345678
void handle_ssid() {
  ssidName =      HTTP.arg("ssid");
  ssidPassword =  HTTP.arg("password");

  jConfig.jWrite("ssidName", ssidName);
  jConfig.jWrite("ssidPassword", ssidPassword);
  //jWrite("subnet", HTTP.arg("subnet"));
  //jWrite("getway", HTTP.arg("getway"));
  //jWrite("dns", HTTP.arg("dns"));
  //jWrite("ip", HTTP.arg("ip"));
  //jWrite("checkboxIP", HTTP.arg("checkboxIP"));
  log("Wifi network changed");
  log("Wifi: " + ssidName);
  log("Password: " + ssidPassword);
  saveConfig();                        // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}
//Установка параметров внутренней точки доступа по запросу вида http://192.168.0.101/ssidap?ssidAP=home1&passwordAP=8765439
void handle_ssidap() {             
  ssidAPName =      HTTP.arg("ssidAP");
  ssidAPPassword =  HTTP.arg("passwordAP");

  jConfig.jWrite("ssidAPName", ssidAPName);     // Получаем значение ssidAP из запроса сохраняем в глобальной переменной
  jConfig.jWrite("ssidAPPassword", ssidAPPassword);  // Получаем значение passwordAP из запроса сохраняем в глобальной переменной
  saveConfig();                         // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}
// Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
void handle_restart() {
  String restart = HTTP.arg("device");          // Получаем значение device из запроса
  if (restart == "ok") {                         // Если значение равно Ок
    HTTP.send(200, "text / plain", "Reset OK"); // Oтправляем ответ Reset OK
    ESP.restart();                                // перезагружаем модуль
  }
  else {                                        // иначе
    HTTP.send(200, "text / plain", "No Reset"); // Oтправляем ответ No Reset
  }
}

void handle_wifi(){
  tries = HTTP.arg("connect").toInt();
  blink = HTTP.arg("blink").toInt();

  jConfig.jWrite("tries", tries);
  jConfig.jWrite("blink", blink);
  HTTP.send(200, "text/plain", "Ok");
  saveConfig();
}
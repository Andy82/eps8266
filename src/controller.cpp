#include <controller.h>
#include <wifi.h>
#include <filesystem.h>
#include <FS.h>
#include <timer.h>
#include <ddns.h>
#include <mqtt.h>
#include <sspd.h>
#include <web.h>


void Controller(void) {

    HTTP.on("/configs.json", [] () {HTTP.send(200, "application/json", jConfig.Serialize());});     // формирование configs.json страницы для передачи данных в web интерфейс

    // API для устройства
    HTTP.on("/wifi.scan.json", handle_wifi_scan);    // сканирование сети на доступные точки доступа
    HTTP.on("/ssdp", handle_ssdp);     // Установить имя SSDP устройства по запросу вида /ssdp?ssdp=proba
    HTTP.on("/ssid", handle_ssid);     // Установить имя и пароль роутера по запросу вида /ssid?ssid=home2&password=12345678
    HTTP.on("/ssidap", handle_ssidap); // Установить имя и пароль для точки доступа по запросу вида /ssidap?ssidAP=home1&passwordAP=8765439
    HTTP.on("/restartWiFi", handle_restartWiFi); //TODO: Implement //Перизапустить wifi попытаться узнать будущий ip адрес перезагрузить устройство
    HTTP.on("/restart", handle_restart);   // Перезагрузка модуля по запросу вида /restart?device=ok
    HTTP.on("/wifi", handle_wifi); //TODO: Implement

    //HTTP страницы для работы с FFS
    HTTP.on("/list", HTTP_GET, handle_FileList); //list directory
    HTTP.on("/edit", HTTP_GET, []() { if (!handle_FileRead("/edit.htm")) HTTP.send(404, "text/plain", "FileNotFound");  });
    HTTP.on("/edit", HTTP_PUT, handle_FileCreate); //Создание файла
    HTTP.on("/edit", HTTP_DELETE, handle_FileDelete); //Удаление файла
    HTTP.on("/edit", HTTP_POST, []() {  HTTP.send(200, "text/plain", ""); }, handle_FileUpload);

    // SSDP дескриптор
    HTTP.on("/description.xml", HTTP_GET, []() {  getSSDP();  });

    HTTP.on("/time", handle_Time);     // Синхронизировать время устройства по запросу вида /Time
    HTTP.on("/timezone", handle_time_zone);    // Установка времянной зоны по запросу вида http://192.168.0.101/TimeZone?timezone=3

    //HTTP страницы для работы с FFS
    HTTP.on("/mqtt", handle_Set_MQTT);
    // Установка параметров ddns
    HTTP.on("/ddns", handle_ddns);

    //Выполнение команды из запроса
    HTTP.on("/cmd", HTTP_GET, []() { sCmd.runCommand(HTTP.arg("command"), HttpCallback);  });

    //called when the url is not defined here
    //use it to load content from SPIFFS
    HTTP.onNotFound([]() { if (!handle_FileRead(HTTP.uri()))  HTTP.send(404, "text/plain", "FileNotFound"); });
}


// сканирование сети на доступные точки доступа
void handle_wifi_scan() {
  long lastScanMillis = 0;
  long currentMillis = millis();
  String jsonWifiNetworks = "";

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  JsonArray& networks = json.createNestedArray("networks");

  // trigger Wi-Fi network scan
  if (currentMillis - lastScanMillis > 10000)
  {
    int n = WiFi.scanNetworks();
    logger.log("Wifi Scan start ... ");
    lastScanMillis = currentMillis;
    logger.log(String(n) + " network(s) found");
    for (int i = 0; i < n; i++) {
      JsonObject& data = networks.createNestedObject();
      data["ssid"] = WiFi.SSID(i);
      data["pass"] = (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "" : "*";
      data["dbm"] = WiFi.RSSI(i);
      data["channel"] = WiFi.channel(i);
      data["bssid"] = WiFi.BSSIDstr(i);
      data["isHidden"] = WiFi.isHidden(i);

      logger.log(String(i+1) + ": " + WiFi.SSID(i).c_str() + ", Ch: " + String(WiFi.channel(i)) + " (" + String(WiFi.RSSI(i)) + " mDb)");
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
  jConfig.Save();               // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
}

// Установка параметров для подключения к внешней AP по запросу вида http://192.168.0.101/ssid?ssid=home2&password=12345678
void handle_ssid() {
  jConfig.jWrite("ssidName", HTTP.arg("ssid"));
  jConfig.jWrite("ssidPassword",  HTTP.arg("password"));
  jConfig.jWrite("subnet", HTTP.arg("subnet"));
  jConfig.jWrite("getway", HTTP.arg("getway"));
  jConfig.jWrite("dns", HTTP.arg("dns"));
  jConfig.jWrite("ip", HTTP.arg("ip"));
  jConfig.jWrite("predefinedIp", HTTP.arg("checkboxIP"));
  logger.log("Wifi network changed");
  jConfig.Save();                           // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

//Установка параметров внутренней точки доступа по запросу вида http://192.168.0.101/ssidap?ssidAP=home1&passwordAP=8765439
void handle_ssidap() {  
  jConfig.jWrite("ssidAPName", HTTP.arg("ssidAP"));
  jConfig.jWrite("ssidAPPassword",  HTTP.arg("passwordAP")); 
  jConfig.Save();
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
  jConfig.jWrite("tries", HTTP.arg("connect"));
  jConfig.jWrite("blink", HTTP.arg("blink"));
  jConfig.Save();
  HTTP.send(200, "text/plain", "Ok");
}

// Перизапустить wifi попытаться узнать будущий ip адрес перезагрузить устройство
void handle_restartWiFi() {
  String ssidName;
  String ssidPassword;
  ssidName =      HTTP.arg("ssid");
  ssidPassword =  HTTP.arg("password");
  //Перезапуск Wi-Fi при первой настройке
  logger.log("WiFi reconnect");
  WiFi.mode(WIFI_AP_STA);
  // Не отключаясь от точки доступа подключаемся к роутеру для получения будущего IP
  WiFi.begin(ssidName.c_str(), ssidPassword.c_str());
  wifiConnect();
  String state = "\{\"title\":\"<h3> Перезагрузка <a href=http://";
  state = state + WiFi.localIP().toString();
  state = state + ">http://" + WiFi.localIP().toString() + "</a></h3>\" \\}";
  logger.log("New IP: " + WiFi.localIP().toString());
  HTTP.send(200, "application/json", state);
  delay(2000);
  // Отключаем точку доступа и переподключаемся к роутеру
  ESP.restart();
}

// Установка параметров ddns
void handle_ddns() {
    jConfig.jWrite("ddns",      HTTP.arg("ddns"));
    jConfig.jWrite("ddnsName",  HTTP.arg("ddnsName"));
    jConfig.jWrite("ddnsPort",  HTTP.arg("ddnsPort"));
    jConfig.Save();
    ip_wan();
    HTTP.send(200, "text/plain", "OK");
}

//Установка параметров  http://192.168.0.101/mqtt?server=m13.cloudmqtt.com&port=15535&user=cxluynva&pass=4cje5WEkzqvR
void handle_Set_MQTT() {           
    jConfig.jWrite("mqttServer", HTTP.arg("server"));
    jConfig.jWrite("mqttPort",   HTTP.arg("port"));
    jConfig.jWrite("mqttUser",   HTTP.arg("user"));
    jConfig.jWrite("mqttPass",   HTTP.arg("pass"));
    jConfig.Save();
    MQTT_Disconnect();
    MQTT_Pablush();
    HTTP.send(200, "text/plain", "OK");  
}

// Установка параметров времянной зоны по запросу вида http://192.168.0.101/TimeZone?timezone=3
void handle_time_zone() {
  jConfig.jWrite("timezone", HTTP.arg("timezone"));
  jConfig.Save();
  HTTP.send(200, "text/plain", "OK");
}

void handle_Time() {
  timeSynch(jConfig.jReadInt("timeZone"));
  HTTP.send(200, "text/plain", "OK"); 
}

bool handle_FileRead(String path) {
  if (path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = HTTP.streamFile(file, contentType);
    logger.log("File " + path +" shared on HTTP " + String(sent) + " bytes");
    file.close();
    return true;
  }
  return false;
}

void handle_FileUpload() {
    // Для файловой системы
  File fsUploadFile;

  if (HTTP.uri() != "/edit") return;
  HTTPUpload& upload = HTTP.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) filename = "/" + filename;
    fsUploadFile = SPIFFS.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    //DBG_OUTPUT_PORT.print("handleFileUpload Data: "); DBG_OUTPUT_PORT.println(upload.currentSize);
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile)
      fsUploadFile.close();
  }
}

void handle_FileDelete() {
  if (HTTP.args() == 0) return HTTP.send(500, "text/plain", "BAD ARGS");
  String path = HTTP.arg(0);
  if (path == "/")
    return HTTP.send(500, "text/plain", "BAD PATH");
  if (!SPIFFS.exists(path))
    return HTTP.send(404, "text/plain", "FileNotFound");
  SPIFFS.remove(path);
  HTTP.send(200, "text/plain", "");
  path = String();
}

void handle_FileCreate() {
  if (HTTP.args() == 0)
    return HTTP.send(500, "text/plain", "BAD ARGS");
  String path = HTTP.arg(0);
  if (path == "/")
    return HTTP.send(500, "text/plain", "BAD PATH");
  if (SPIFFS.exists(path))
    return HTTP.send(500, "text/plain", "FILE EXISTS");
  File file = SPIFFS.open(path, "w");
  if (file)
    file.close();
  else
    return HTTP.send(500, "text/plain", "CREATE FAILED");
  HTTP.send(200, "text/plain", "");
  path = String();
}

void handle_FileList() {
  if (!HTTP.hasArg("dir")) {
    HTTP.send(500, "text/plain", "BAD ARGS");
    return;
  }
  String path = HTTP.arg("dir");
  Dir dir = SPIFFS.openDir(path);
  path = String();
  String output = "[";
  while (dir.next()) {
    File entry = dir.openFile("r");
    if (output != "[") output += ',';
    bool isDir = false;
    output += "{\"type\":\"";
    output += (isDir) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += String(entry.name()).substring(1);
    output += "\"}";
    entry.close();
  }
  output += "]";
  HTTP.send(200, "application/json", output);
}
#include <PubSubClient.h>
String prefix   = "/IoTmanager";

// Для поиска других устройств по протоколу SSDP
WiFiClient wclient;
PubSubClient client(wclient);

String mqttServer;
int mqttPort;
String mqttUser;
String mqttPass;

void MQTT_init() {
    mqttServer  = jConfig.jRead("mqttServer");
    mqttPort    = jConfig.jRead("mqttPort").toInt();
    mqttUser    = jConfig.jRead("mqttUser");
    mqttPass    = jConfig.jRead("mqttPass");

  MQTT_Pablush();
  if (client.connected()) {
    log("Connected to MQTT");
  } else {
    log("Can't connect MQTT server");
  }
}

void  MQTT_loop() {
  if (client.connected()) {
    client.loop();
  }
  else
  {
    MQTT_Pablush();
  }
    yield();
}

// Функция получения данных от сервера
void callback(const MQTT::Publish& pub)
{
  logFlash(1, 500);
  // выводим в сериал порт название топика и значение полученных данных
  log(pub.topic() + " => " + pub.payload_string());
  String payload = pub.payload_string();

  if (String(pub.topic()) =="cmd") {
    sCmd.runCommand(String(payload), MQTT_Send);
  }

  if ( String(pub.payload_string()) == "HELLOW" ) {
    loadnWidgets();
  }

  if (String(pub.topic()) == prefix + "/" + chipID + "/RELE_1_not/control") // проверяем из нужного ли нам топика пришли данные
  {
    int stled = payload.toInt(); // преобразуем полученные данные в тип integer
    //if (stled == 1)   command = "relayon";
    //if (stled == 0)   command = "relayoff";
  }
}




void MQTT_Pablush() {
  if ((mqttServer != "")) {
    client.set_server(mqttServer, mqttPort);
    // подключаемся к MQTT серверу
    if (WiFi.status() == WL_CONNECTED) {
      if (!client.connected()) {
        if (client.connect(MQTT::Connect(chipID).set_auth(mqttUser, mqttPass))) {
          client.set_callback(callback);
          client.subscribe(prefix);  // Для приема получения HELLOW и подтверждения связи
          client.subscribe("cmd"); 
          client.subscribe(prefix + "/+/+/control"); // Подписываемся на топики control
          //client.subscribe("/" + chipID + "/RELE_1"); // подписываемся по топик с данными для светодиода
          loadnWidgets();
        } else {
        }
      }
    }
  }
}


void MQTT_Send(String command)
{
    client.publish("test/temp", command);
}

/*
int tm=300;

void MQTT_Send()
{
  if (tm==0)
  {
    String temp = "Testing";
    client.publish("test/temp",temp);
    Serial.println(temp);
    tm = 300;
  }
  tm--;
  delay(10);
}
*/

void MQTT_TestSend()
{
    client.publish("test/temp","Testing MQTT");
}


//Установка параметров  http://192.168.0.101/mqtt?server=m13.cloudmqtt.com&port=15535&user=cxluynva&pass=4cje5WEkzqvR
void handle_Set_MQTT() {              
    mqttServer  = HTTP.arg("server");
    mqttPort    = HTTP.arg("port").toInt();
    mqttUser    = HTTP.arg("user");
    mqttPass    = HTTP.arg("pass");

    jConfig.jWrite("mqttServer", mqttServer);
    jConfig.jWrite("mqttPort",   mqttPort);
    jConfig.jWrite("mqttUser",   mqttUser);
    jConfig.jWrite("mqttPass",   mqttPass);
    saveConfig();
    client.disconnect();
    MQTT_Pablush();
    HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}



// Читаем и отправляем виджеты на сервер
bool loadnWidgets() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& Widgets = jsonBuffer.parseObject(readFile("config.widgets.json", 4096));
  JsonArray& WidgetsArray = Widgets["nWidgets"];
  int j = WidgetsArray.size();
  if (j != 0) {
    for (int i = 0; i <= j - 1; i++) {
      Widgets["nWidgets"][i]["topic"] = prefix + "/" + chipID + Widgets["nWidgets"][i]["topic"].as<String>();
      Widgets["nWidgets"][i]["descr"] = jConfig.jRead("SSDP");
      String thing_config = Widgets["nWidgets"][i].as<String>();
      client.publish(MQTT::Publish(prefix + "/" + chipID + "/config", thing_config).set_qos(1));
    }
  }
  return true;
}

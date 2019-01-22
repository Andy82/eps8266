#include <PubSubClient.h>
#include <mqtt.h>

/*
 * ESPWebMQTTBase class implementation
 */

ESPMQTT::ESPMQTT() {
  _espClient = new WiFiClient();
  pubSubClient = new PubSubClient(*_espClient);
}

ESPMQTT::~ESPMQTT() {
  pubSubClient->disconnect();
}


void ESPMQTT::setup() {
  _mqttServer    = jConfig.jRead("mqttServer");
  _mqttPort      = jConfig.jRead("mqttPort").toInt();
  _mqttUser      = jConfig.jRead("mqttUser");
  _mqttPassword  = jConfig.jRead("mqttPass");
  _mqttClient        = jConfig.jRead("chipID");

  logger.log("mqttServer =" + _mqttServer); 
  logger.log("mqttPort =" + String(_mqttPort)); 
  logger.log("mqttUser =" + String(_mqttUser)); 
  logger.log("mqttPass =" + String(_mqttPassword)); 
  logger.log("mqttId =" + String(_mqttClient)); 

  if (_mqttServer.length() > 0 && WiFi.status() == WL_CONNECTED)  {
    pubSubClient->setServer((char *)_mqttServer.c_str(), _mqttPort);
    // подключаемся к MQTT серверу
      if (!pubSubClient->connected()) 
      {
          if (pubSubClient->connect((char *)_mqttClient.c_str(), (char *)_mqttUser.c_str(), (char *)_mqttPassword.c_str())) 
          {
            pubSubClient->setCallback(std::bind(&ESPMQTT::mqttCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
          } 
    }
  }
  if (pubSubClient->connected()) {
      logger.log("Connected to MQTT");
    } else {
      logger.log("Can't connect MQTT server");
  }
}

void ESPMQTT::loop() {
  if ((_mqttServer != "") && ((WiFi.getMode() == WIFI_STA) && (WiFi.status() == WL_CONNECTED))) {
    if (! pubSubClient->connected())
      logger.log("Reconnecting to the MQTT server");
      mqttReconnect();
    if (pubSubClient->connected())
      pubSubClient->loop();
  }
}


void ESPMQTT::mqttCallback(char* topic, byte* payload, unsigned int length) {
  logger.logFlash(1, 500);
  // выводим в сериал порт название топика и значение полученных данных
  logger.log("Message arrived [" + String(topic) + " ]=> ");
  String sPayload = String((char *)payload);
  logger.log(sPayload);
  //if (String(topic) =="cmd") {
  //if ((char)payload[0] == '1')
}

bool ESPMQTT::mqttReconnect() {
  const uint32_t timeout = 30000;

  static uint32_t nextTime;
  bool result = false;

  if ((int32_t)(millis() - nextTime) >= 0) {
    logger.log(F("Attempting MQTT connection..."));
    //enablePulse(PULSE);
    if (_mqttUser != "")
      result =  pubSubClient->connect(_mqttClient.c_str(), _mqttUser.c_str(), _mqttPassword.c_str());
    else
      result = pubSubClient->connect(_mqttClient.c_str());
//    enablePulse(WiFi.getMode() == WIFI_STA ? BREATH : FADEIN);
    //enablePulse(BREATH);
    if (result) {
      logger.log(F(" connected"));
      mqttResubscribe();
    } else {
      logger.log(" failed, rc=" + String(pubSubClient->state()));
    }
    nextTime = millis() + timeout;
  }

  return result;
}


void ESPMQTT::mqttResubscribe() {
  String topic;

  if (_mqttClient != "") {
    topic += "/IoTmanager/";
    topic += _mqttClient;
    topic += F("/#");
    mqttSubscribe(topic);
  }
}

bool ESPMQTT::mqttSubscribe(const String& topic) {
    logger.log("MQTT subscribe to " + topic);
    return pubSubClient->subscribe(topic.c_str());
}

bool ESPMQTT::mqttPublish(const String& topic, const String& value) {
    logger.log("MQTT publish topic " + topic + " vaule: " + value);
    return pubSubClient->publish(topic.c_str(), value.c_str());
}
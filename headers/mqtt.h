#ifndef MQTT_H
#define MQTT_H
#include "globals.h"
#include <PubSubClient.h>

class ESPMQTT { // Расширение базового класса с поддержкой MQTT
public:
  ESPMQTT();
  ~ESPMQTT();
  void setup();
  void loop();  
  bool mqttReconnect(); // Восстановление соединения с MQTT-брокером
  void mqttResubscribe(); // Осуществление подписки на топики
  bool mqttSubscribe(const String& topic); // Хэлпер для подписки на топик
  bool mqttPublish(const String& topic, const String& value); // Хэлпер для публикации топика

protected:  
  PubSubClient* pubSubClient; // Клиент MQTT-брокера
  void mqttCallback(char* topic, byte* payload, unsigned int length); // Callback-функция, вызываемая MQTT-брокером при получении топика, на которое оформлена подписка
  WiFiClient* _espClient;
  String _mqttServer; // MQTT-брокер
  uint16_t _mqttPort; // Порт MQTT-брокера
  String _mqttUser; // Имя пользователя для авторизации
  String _mqttPassword; // Пароль для авторизации
  String _mqttClient; // Имя клиента для MQTT-брокера (используется при формировании имени топика для публикации в целях различия между несколькими клиентами с идентичным скетчем)
};
#endif
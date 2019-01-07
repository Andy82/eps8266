#include <PubSubClient.h>
#include <mqtt.h>
#include <StringCommand.h>

const char* prefix   = "/IoTmanager";

// Для поиска других устройств по протоколу SSDP
WiFiClient espClient;
PubSubClient client(espClient);

String mqttServer;
int mqttPort;
String mqttId;
String mqttUser;
String mqttPass;

void MQTT_init() 
{
  mqttServer    = jConfig.jRead("mqttServer");
  mqttPort      = jConfig.jRead("mqttPort").toInt();
  mqttUser      = jConfig.jRead("mqttUser");
  mqttPass      = jConfig.jRead("mqttPass");
  mqttId        = jConfig.jRead("chipID");

  logger.log("mqttServer =" + mqttServer); 
  logger.log("mqttPort =" + String(mqttPort)); 
  logger.log("mqttUser =" + String(mqttUser)); 
  logger.log("mqttPass =" + String(mqttPass)); 
  logger.log("mqttId =" + String(mqttId)); 

  MQTT_Pablush();
  if (client.connected()) {
    logger.log("Connected to MQTT");
  } else {
    logger.log("Can't connect MQTT server");
  }
}

void  MQTT_loop() {
  if (client.connected()) {
    client.loop();
  }
  else
  {
    //TODO: Set delay
    MQTT_Pablush();
  }
}


void callback(char* topic, byte* payload, unsigned int length) {
  logger.logFlash(1, 500);
  // выводим в сериал порт название топика и значение полученных данных
  logger.log("Message arrived [" + String(topic) + " ]=> ");

  String sPayload = String((char *)payload);
  logger.log(sPayload);

  if (String(topic) =="cmd") {
    sCmd.runCommand(sPayload, MQTT_Send);
  }

  if (String(topic) == String(prefix) + "/" + mqttId + "/RELE_1_not/control") // проверяем из нужного ли нам топика пришли данные
  {
    if ((char)payload[0] == '1')
    {}

  }
}



void MQTT_Pablush() {
  if (mqttServer.length() > 0)
  {
    client.setServer((char *)mqttServer.c_str(), mqttPort);
    // подключаемся к MQTT серверу
    if (WiFi.status() == WL_CONNECTED) 
    {
      if (!client.connected()) 
      {
          if (client.connect((char *)mqttId.c_str(), (char *)mqttUser.c_str(), (char *)mqttPass.c_str())) {
          client.setCallback(callback);
          client.subscribe(prefix);  // Для приема получения HELLOW и подтверждения связи
          client.subscribe("cmd"); 
          client.subscribe((String(prefix) + "/+/+/control").c_str()); // Подписываемся на топики control
        } 
      }
    }
  }
}

void MQTT_Disconnect()
{
  client.disconnect();
}


void MQTT_Send(String command)
{
    client.publish("test/temp", command.c_str());
}


void MQTT_TestSend()
{
    client.publish("test/temp","Testing MQTT");
}



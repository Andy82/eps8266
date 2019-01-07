#ifndef MQTT_H
#define MQTT_H
#include "globals.h"
#include <StringCommand.h>
#include <PubSubClient.h>

void MQTT_init();
void MQTT_loop();
void callback(char* topic, byte* payload, unsigned int length) ;
void MQTT_Pablush();
void MQTT_Send(String command);
void MQTT_TestSend();
void MQTT_Disconnect();


extern StringCommand sCmd;

#endif
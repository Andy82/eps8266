#ifndef WIFI_H
#define WIFI_H
#include "globals.h"

void WIFI_init();
void handle_restartWiFi();
boolean startSTA();
boolean wifiConnect();
boolean startAP();

#endif
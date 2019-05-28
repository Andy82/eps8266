#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "globals.h"

void Controller(void);
void handle_wifi_scan();
void handle_ssdp();
void handle_ssid();
void handle_ssidap();
void handle_restart();
void handle_wifi();
void handle_ddns();
void handle_Set_MQTT();
void handle_time_zone();
void handle_Time();
bool handle_FileRead(String path);
void handle_FileUpload();
void handle_FileDelete();
void handle_FileCreate();
void handle_FileList();
#endif
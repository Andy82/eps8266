#include <globals.h>
#include <configurations.h>
#include <ftp.h>
#include <commands.h>
#include <controller.h>
#include <ddns.h>
#include <wifi.h>
#include <hardware.h>
#include <timer.h>
#include <log.h>
#include <filesystem.h>
#include <sspd.h>
//#include <telegram.h>
#include <telnet.h>
#include <timeticker.h>
#include <web.h>
#include <websocket.h>
//#include <update.h>
#include <mqtt.h>
#include <mdns.h>


void setup() {
  logger.log("================= Init started =================");
  Hardware_init();
  FS_init();
  Config_init();
  WIFI_init();
  Telnet_Init();
  Time_init();
  Cmd_init();
  //Telegram_init();
  Controller();
  HTTP_init();
  SSDP_init();
  Ftp_init();
  MQTT_init();
  DDNS_init();
  //Update_init();
  WebSocket_init();
  mDNS_init();
  TimeTicker_init();
  Config_updateSysytemParameters();
  logger.log("================= Init completed =================");
  
}

void loop() {
  Telnet_loop();
  Cmd_loop();
  HTTP_loop();
  MQTT_loop();
  Ftp_Loop();
  WebSocket_loop();
  //Telegram_loop();
  TimeTicker_loop();
  logger.Serial_loop();
  yield();
}
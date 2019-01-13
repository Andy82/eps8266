#include "globals.h"

LogClass logger(&Serial, true);
Json jConfig("configs.json");
ESP8266WebServer HTTP(80);
ESPMQTT mqtt;
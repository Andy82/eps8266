#include <ESP8266WebServer.h>
#include "json.h"
#include "log.h"
#include "mqtt.h"
class ESPMQTT;

extern LogClass logger;
extern Json jConfig;
extern ESP8266WebServer HTTP;
extern ESPMQTT mqtt;
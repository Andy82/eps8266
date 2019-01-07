#include <ESP8266SSDP.h>             //Содержится в пакете
#include <ESP8266WebServer.h>
#include <sspd.h>


void SSDP_init(void) {
  //Если версия  2.0.0 закаментируйте следующую строчку
  SSDP.setDeviceType("upnp:rootdevice");
  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(jConfig.jReadInt("httpPort"));
  SSDP.setName(jConfig.jRead("deviceName"));
  SSDP.setSerialNumber(jConfig.jRead("serialNumber"));
  SSDP.setURL("/");
  SSDP.setModelName(jConfig.jRead("modelName"));
  SSDP.setModelNumber(jConfig.jRead("modelNumber"));
  SSDP.setModelURL(jConfig.jRead("modelURL"));
  SSDP.setManufacturer(jConfig.jRead("manufacturer"));
  SSDP.setManufacturerURL(jConfig.jRead("manufacturerURL"));
  SSDP.begin();
}

void getSSDP()
{
   SSDP.schema(HTTP.client());
}
#include <mdns.h>
#include <ESP8266mDNS.h>  

void mDNS_init()
{
    String deviceName;

    deviceName = jConfig.jRead("deviceName");
    deviceName.toLowerCase();
    if (!MDNS.begin((char *)deviceName.c_str())) 
    {   // Start the mDNS responder for esp8266.local
      logger.log("Error setting up MDNS responder!");
    } else
    {
      logger.log("MDNS server started for " + deviceName + ".local");
    }


      // Add service to MDNS-SD
      MDNS.addService("http", "tcp", 80);
}

void mDNS_loop() {
  MDNS.update();
}
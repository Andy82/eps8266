#include <mdns.h>
#include <ESP8266mDNS.h>  

void mDNS_init()
{
    if (!MDNS.begin((char *)jConfig.jRead("deviceName").c_str())) 
    {   // Start the mDNS responder for esp8266.local
      logger.log("Error setting up MDNS responder!");
    }
}

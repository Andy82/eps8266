#include <ESP8266WiFi.h>
#include <telnet.h>

WiFiServer telnetServer(23);
WiFiClient serverClient;
String lastLogRow = "";
uint16_t lastLogRowId = 0;

unsigned long startTime = millis();

void Telnet_Init()
{
    telnetServer.begin();
    telnetServer.setNoDelay(true);
    logger.log("Telnet Client , exit with ^] and 'quit'");
}

void Telnet_loop() {
  // look for Client connect trial
  if (telnetServer.hasClient()) {
    if (!serverClient || !serverClient.connected()) {
      if (serverClient) {
        serverClient.stop();
        logger.log("Telnet Client Stop");
      }
      serverClient = telnetServer.available();
      logger.log("New Telnet client");
      serverClient.flush();  // clear input buffer, else you get strange characters 
    }
  }
    while(serverClient.available()) 
    {  // get data from Client
        Serial.write(serverClient.read());
    }

  if (millis() - startTime > 5000) { // run every 5000 ms
    startTime = millis();

    if (serverClient && serverClient.connected()) 
    {  // send data to Client
      Telnet_PrintLog();
    }
  }
  delay(10);  // to avoid strange characters left in buffer
}

void Telnet_PrintLog()
{
  uint16_t len = logger.lines();

  while (logger.line(len) == "" && len >0)
  {
    len--;
  }

  if (lastLogRowId == len && logger.line(lastLogRowId) != lastLogRow )
  {
    while (logger.line(lastLogRowId) != lastLogRow && lastLogRowId>0)
    {
      lastLogRowId--;
    }
  }

  for (uint16_t i=lastLogRowId; i<len; i++)
  {
      serverClient.println(logger.line(i));
  }

  lastLogRowId = len;
  lastLogRow = logger.line(lastLogRowId);
}
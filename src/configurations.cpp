#include <filesystem.h>

String chipID;

void Config_init()
{
  logger.log("Configuration read from : " + jConfig.getFileName());
  chipID = String( ESP.getChipId() ) + "-" + String( ESP.getFlashChipId());
  jConfig.Desirialize(readFile(jConfig.getFileName(), 4096));
  logger.log("Configuration read completed.");
}

void Config_updateSysytemParameters()
{
  logger.log("Updating system parameters");
  jConfig.jWrite("mac", WiFi.macAddress().c_str());
  jConfig.jWrite("ip", WiFi.localIP().toString());
  jConfig.jWrite("chipID", chipID); 
  jConfig.jWrite("sketchMD5", ESP.getSketchMD5()); 
  //jConfig.Save();
}

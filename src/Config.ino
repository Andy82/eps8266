Json jConfig("configs.json");

String chipID;

void Config_init()
{
  chipID = String( ESP.getChipId() ) + "-" + String( ESP.getFlashChipId());
  jConfig.Desirialize(readFile(jConfig.getFileName(), 4096));
  jConfig.jWrite("mac", WiFi.macAddress().c_str());
  jConfig.jWrite("ip", WiFi.localIP().toString());
  jConfig.jWrite("time", GetTime());
  jConfig.jWrite("chipID", chipID);
}

void saveConfig (){
  jConfig.Print();
  writeFile(jConfig.getFileName(), jConfig.Serialize());
}

void handle_ConfigJSON() {
  jConfig.jWrite("time", GetTime());
  saveConfig();
  HTTP.send(200, "application/json", jConfig.Serialize());
}

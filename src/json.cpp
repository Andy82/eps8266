#include <json.h>
#include <filesystem.h>
#include <ArduinoJson.h>

Json::Json(String fname) : jsonConfig("{}")
{
    this->fileName = fname;
}

// Чтение значения json
String Json::jRead(String name) {
  DynamicJsonDocument jsonBuffer(1024);
  deserializeJson(jsonBuffer, this->jsonConfig);
  //Serial.println("jRead " + name + ":" + root[name].as<String>());
   return jsonBuffer[name].as<String>();
}

int Json::jReadInt(String name) {
  DynamicJsonDocument  jsonBuffer(1024);
  deserializeJson(jsonBuffer, this->jsonConfig);
  //Serial.println("jRead " + name + ":" + root[name].as<String>());
   return jsonBuffer[name].as<int>();
}

// Запись значения json String
void Json::jWrite(String name, String volume) {
  DynamicJsonDocument  jsonBuffer(1024);
  deserializeJson(jsonBuffer, this->jsonConfig);
  jsonBuffer[name] = volume;
  this->jsonConfig = "";
  serializeJsonPretty(jsonBuffer, this->jsonConfig);
}

void Json::jWrite(String name, int volume) {
    jWrite(name, String(volume));
}

String Json::jCreate(String &json, String name, String volume) {
  DynamicJsonDocument  jsonBuffer(1024);
  deserializeJson(jsonBuffer, this->jsonConfig);
  jsonBuffer[name] = volume;
  json = "";
  serializeJsonPretty(jsonBuffer, json);
  return json;
}


void Json::Desirialize(String data){
    this->jsonConfig = data;
    DynamicJsonDocument  jsonBuffer(1024);
    DeserializationError desError = deserializeJson(jsonBuffer, this->jsonConfig);
    if (!desError) {
        logger.log("ERROR: Can't parse JSON:");
    } else {
        logger.log("JSON successfully parsed");
    }
}

String Json::Serialize()
{
    return this->jsonConfig;
}

void Json::Print(){
    DynamicJsonDocument  jsonBuffer(1024);
    deserializeJson(jsonBuffer, this->jsonConfig);
    serializeJsonPretty(jsonBuffer, Serial);
}

String Json::PrintToString()
{
    DynamicJsonDocument  jsonBuffer(1024);
    deserializeJson(jsonBuffer, this->jsonConfig);
    String resultString;
    serializeJsonPretty(jsonBuffer, resultString);
    return resultString;
}

void Json::Save(){
    Print();
    writeFile(getFileName(), PrintToString());
}

// Вывести имя файла
String Json::getFileName() {
    return this->fileName;
}
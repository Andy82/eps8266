#include <json.h>
#include <filesystem.h>
#include <ArduinoJson.h>

Json::Json(String fname) : jsonConfig("{}")
{
    this->fileName = fname;
}

// Чтение значения json
String Json::jRead(String name) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(this->jsonConfig);
  //Serial.println("jRead " + name + ":" + root[name].as<String>());
   return root[name].as<String>();
}

int Json::jReadInt(String name) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(this->jsonConfig);
  //Serial.println("jRead " + name + ":" + root[name].as<String>());
   return root[name].as<int>();
}

// Запись значения json String
void Json::jWrite(String name, String volume) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(this->jsonConfig);
  root[name] = volume;
  this->jsonConfig = "";
  root.printTo(this->jsonConfig);
}

void Json::jWrite(String name, int volume) {
    jWrite(name, String(volume));
}

String Json::jCreate(String &json, String name, String volume) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  root[name] = volume;
  json = "";
  root.printTo(json);
  return json;
}


void Json::Desirialize(String data){
    this->jsonConfig = data;
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(data);
    if (root.size() == 0) {
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
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(this->jsonConfig);
    root.prettyPrintTo(Serial);
}

String Json::PrintToString()
{
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(this->jsonConfig);
    String resultString;
    root.prettyPrintTo(resultString);
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
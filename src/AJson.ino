#include <ArduinoJson.h>

class Json {
private:
    String fileName;
    String jsonConfig;
public:
    Json(String fname);
    String jRead(String name);
    void jWrite(String name, String volume);
    void jWrite(String name, int volume);
    String Serialize();
    void Desirialize(String data);
    void Print();
    String getFileName();
};

Json::Json(String fname) : jsonConfig("{}")
{
    this->fileName = fname;
}

// ------------- Чтение значения json
String Json::jRead(String name) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(this->jsonConfig);
  return root[name].as<String>();
}

// ------------- Запись значения json String
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


void Json::Desirialize(String data){
    this->jsonConfig = data;
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


// ------------- Вывести имя файла
String Json::getFileName() {
    return this->fileName;
}
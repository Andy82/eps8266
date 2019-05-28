#ifndef JSON_H
#define JSON_H
#include <ArduinoJson.h>

class Json {
private:
    String fileName;
    String jsonConfig;
public:
    Json(String fname);
    String jRead(String name);
    int jReadInt(String name);
    void jWrite(String name, String volume);
    void jWrite(String name, int volume);
    String jCreate(String &json, String name, String volume);
    String Serialize();
    void Desirialize(String data);
    void Print();
    void Save();
    String PrintToString();
    String getFileName();
};

#endif
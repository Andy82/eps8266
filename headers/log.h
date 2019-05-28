#ifndef LOGCLASS_H
#define LOGCLASS_H
#include <Arduino.h>

const uint16_t maxLogSize = 2048 - 1; // Maximum size of log in characters

class LogClass {
private:
    bool DEBUG;
    void addLogToString(String text);
    void init_flash();
public:
    LogClass(const Stream* duplicate, bool debug);
    void logFlash(int times, int ms);
    void log(String text);
    void log(const char *text);

    void clear() 
    {
        _log = "";
    }

    uint16_t lines();

    const String& get_log() const 
    {
        return _log;
    }
    
    String line(uint16_t index);
    String operator[](uint16_t index) 
    {
        return line(index);
    }

    void Serial_loop();

protected:
    String _log;
    Stream* _duplicate;
};

#endif
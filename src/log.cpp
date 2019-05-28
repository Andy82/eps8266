#include <log.h>
#include <timer.h> 

#define LED1 3  
#define LED2 2 

LogClass::LogClass(const Stream* duplicate = NULL, bool debug = true)
{
    this->DEBUG = debug;
    Serial.begin(115200);
    _log = "";
    _log.reserve(maxLogSize);
    delay(500);
    Serial.println("Connected");
    init_flash();
}

void LogClass::init_flash()
{
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    logFlash(5, 500);
}


void LogClass::log(String text)
{
  if (DEBUG)
    Serial.println(String(GetTime() + " | " + text));
    addLogToString(GetTime() + " | " + text + '\n');
}

void LogClass::log(const char *text)
{
  if (DEBUG)
    Serial.println(String(millis()) + " | " + GetTime() + " | " + text);
    addLogToString(GetTime() + " | " + text + '\n');
}

void LogClass::addLogToString(String text)
{
    uint16_t len = _log.length();

    if (len >= maxLogSize) { // Compacting log by removing first line or half of log
      uint16_t i = 0;

      while ((i < len) && (_log[i] != '\n')) // Find first new line character
        ++i;
      ++i;
      if (i < len)
        _log.remove(0, i);
      else
        _log.remove(0, maxLogSize / 2);
    }
    _log += text;
}


void LogClass::logFlash(int times, int ms)
{
    for (int i = 0; i <times; i++)
    {
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
        delay(ms);
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, HIGH);
        delay(ms);
    }
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
}

void LogClass::Serial_loop()
{
    if (Serial.available() > 0){
        Serial.println(Serial.readString());
    }
}

uint16_t LogClass::lines() {
  uint16_t result = 0;
  uint16_t len = _log.length();

  for (uint16_t i = 0; i < len; ++i) {
    if (_log[i] == '\n')
      ++result;
  }
  if ((len > 0) && (_log[len - 1] != '\n')) // Counting last line if it is not empty
    ++result;

  return result;
}

String LogClass::line(uint16_t index) {
  String result;
  uint16_t len = _log.length();
  uint16_t startPos, endPos;

  startPos = 0;
  while (index > 0) { // Skip (index - 1) lines
    while ((startPos < len) && (_log[startPos] != '\n')) // Find new line character
      ++startPos;
    if (startPos < len) { // Found new line character
      ++startPos; // First character on new line
      --index;
    } else
      break;
  }
  if (startPos < len) {
    endPos = startPos;
    while ((endPos < len) && (_log[endPos] != '\n')) // Find next new line character
      ++endPos;
    result = _log.substring(startPos, endPos);
  }
  return result;
}
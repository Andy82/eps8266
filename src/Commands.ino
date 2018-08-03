#include <StringCommand.h>
StringCommand sCmd;

void Cmd_init()
{
    //addCommand("mqtt", MQTT_TestSend);
    sCmd.addCommand("print", printTest);
    sCmd.setDefaultHandler(unrecognized);
}

void Cmd_loop()
{
  if (sCmd.readyToExecute)
  {
    log("Command: " + sCmd.commandName);
    sCmd.readStr();  
  }
}

String printTest(char *attrib) {
  return ("Test " + String(attrib) + "!!!");
}

void unrecognized(const char *command) {
  Serial.println("What?");
}
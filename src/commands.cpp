#include <StringCommand.h>
#include <commands.h>

StringCommand sCmd;

void Cmd_init()
{
    //sCmd.addCommand("mqtt", MQTT_TestSend);
    sCmd.addCommand("print", printTest);
    sCmd.setDefaultHandler(unrecognized);
}

void Cmd_loop()
{
  if (sCmd.readyToExecute)
  {
    logger.log("Command: " + sCmd.commandName);
    sCmd.readStr();  
  }
}

String printTest(char *attrib) {
  return ("Test " + String(attrib) + "!!!");
}

void unrecognized(const char *command) {
  logger.log("Command " + String(command) + " not recodnized.");
}
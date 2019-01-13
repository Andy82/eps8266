#ifndef COMMANDS_H
#define COMMANDS_H
#include "globals.h"

void Cmd_init();
void Cmd_loop();
String printTest(char *attrib);
void unrecognized(const char *command);
void runCommand(String command, void (*callbackFunction)(String));

#endif
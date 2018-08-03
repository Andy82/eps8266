/**
 * SerialCommand - A Wiring/Arduino library to tokenize and parse commands
 * received over a serial port.
 *
 * Copyright (C) 2012 Stefan Rado
 * Copyright (C) 2011 Steven Cogswell <steven.cogswell@gmail.com>
 *                    http://husks.wordpress.com
 *
 * Version 20120522
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef StringCommand_h
#define StringCommand_h

#if defined(WIRING) && WIRING >= 100
  #include <Wiring.h>
#elif defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif
#include <string.h>

// Size of the input buffer in bytes (maximum length of one command plus arguments)
#define SERIALCOMMAND_BUFFER 32
// Maximum length of a command excluding the terminating null
#define SERIALCOMMAND_MAXCOMMANDLENGTH 16

// Uncomment the next line to run the library in debug mode (verbose messages)
//#define SERIALCOMMAND_DEBUG


class StringCommand {
  public:
    StringCommand();      // Constructor
    // Current command name
    String commandName;    
    void runCommand(String command, void (*callbackFunction)(String)); // Add command to the run list
    // Flag that already have command to run
    bool readyToExecute;
    void addCommand(const char *command, String (*function)(char *));  // Add a command to the processing dictionary.
    void setDefaultHandler(void (*function)(const char *));   // A handler to call when no valid command received.

    void readStr();    // Main entry point.
    void clearBuffer();   // Clears the input buffer.

  private:
    // Callback function
    void (*commandCallback)(String);
    // Command/handler dictionary
    struct StringCommandCallback {
      char command[SERIALCOMMAND_MAXCOMMANDLENGTH + 1];
      String (*function)(char *);
    };                                    // Data structure to hold Command/Handler function key-value pairs
    StringCommandCallback *commandList;   // Actual definition for command/handler array
    byte commandCount;

    // Pointer to the default handler function
    void (*defaultHandler)(const char *);

    char delim[2]; // null-terminated list of character to be used as delimeters for tokenizing (default " ")

    char buffer[SERIALCOMMAND_BUFFER + 1]; // Buffer of stored characters while waiting for terminator character
    byte bufPos;                        // Current position in the buffer
    char *last;                         // State variable used by strtok_r during processing
};

#endif //StringCommand_h

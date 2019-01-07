#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include "globals.h"

void FS_init();
String getContentType(String filename);
String readFile(String fileName, size_t len );
String writeFile(String fileName, String data);
String formatBytes(size_t bytes);

#endif
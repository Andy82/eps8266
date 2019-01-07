#ifndef WEB_H
#define WEB_H
#include "globals.h"

void HTTP_init(void);
void HTTP_loop();
void HttpCallback(String command);

#endif
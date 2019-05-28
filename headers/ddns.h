#ifndef DDNS_H
#define DDNS_H
#include "globals.h"

void DDNS_init();
void DDNS_loop();
void ip_wan();
String getURL(String urls);

#endif


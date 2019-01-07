#ifndef WEBSOCKET_H
#define WEBSOCKET_H
#include "globals.h"
#include <WebSocketsServer.h>  

void WebSocket_init();
void WebSocket_loop();
void WebSocket_Event(uint8_t num, WStype_t type, uint8_t * payload, size_t length) ;
void WebSocket_Send (String key, String data) ;

#endif
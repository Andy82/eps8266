#include <websocket.h>
#include <WebSocketsServer.h>  
#include <timer.h> 

// Объект для  webSocket
WebSocketsServer webSocket = WebSocketsServer(81);

void WebSocket_init()
{
  webSocket.begin();
  webSocket.onEvent(WebSocket_Event);


}


void WebSocket_loop()
{
    webSocket.loop();
}


void WebSocket_Event(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:  // Событие происходит при отключени клиента 
      logger.log("WebSocket disconnected");
      break;
    case WStype_CONNECTED: // Событие происходит при подключении клиента
      {
        logger.log("WebSocket Connected"); 
        IPAddress ip = webSocket.remoteIP(num);
        logger.log("Connected from "+ String(ip.toString()) +" url: " + String((char *)payload));

        webSocket.sendTXT(num, jConfig.PrintToString().c_str()); // Отправим в всю строку с данными используя номер клиента он в num
      }
      break;
    case WStype_TEXT: // Событие происходит при получении данных текстового формата из webSocket
      // webSocket.sendTXT(num, "message here"); // Можно отправлять любое сообщение как строку по номеру соединения
      // webSocket.broadcastTXT("message here");
      logger.log("Websocket received test!!!" + String((char *)payload));
      break;
    case WStype_BIN:      // Событие происходит при получении бинарных данных из webSocket
      // webSocket.sendBIN(num, payload, length);
      break;
    default :
      break;
  }
}
// Отправка данных в Socket всем получателям
// Параметры Имя ключа, Данные, Предыдущее значение
void WebSocket_Send (String key, String data)  {
    String broadcast = "{}";
    jConfig.jCreate(broadcast, key, data);
    webSocket.broadcastTXT(broadcast);
}


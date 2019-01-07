#include <TickerScheduler.h>         //https://github.com/Toshik/TickerScheduler
#include <timeticker.h>
#include <ddns.h>
#include <telegram.h>
#include <WebSockets.h>

TickerScheduler timeScheduler(4);

void TimeTicker_init()
{
    // задача синхронизайия с сервером ddns каждые 5 минут
    //timeScheduler.add(1, 300000, [&](void*) { ip_wan(); }, nullptr, true);
    //timeScheduler.add(2, 30000, [&](void*) { Telegram_loop(); }, nullptr, true);

    /*
  timeScheduler.add(3, 1000, [&](void*) { // Запустим задачу 1 с интервалом 1000ms
    String timeX = GetTime();
    WebSocket_Send ("time", timeX);
    logger.log("Websocket send : time = " + timeX);
  }, nullptr, true);
  */
}

void TimeTicker_loop()
{
    timeScheduler.update();
}
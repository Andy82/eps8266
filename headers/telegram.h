#ifndef TELEGRAM_H
#define TELEGRAM_H
#include "globals.h"

#include <StringCommand.h>
#include <TickerScheduler.h>

void Telegram_init();
void Telegram_loop();
void SendTelegramMessage(String command);

extern StringCommand sCmd;

#endif
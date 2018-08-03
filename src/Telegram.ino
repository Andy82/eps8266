#include <TelegramBot.h>

// Initialize Telegram BOT
const char BotToken[] = "456542906:AAFzslYVZIEoIwIlh6eos_dNjBVH3MSHv3s";

WiFiClientSecure net_ssl;
TelegramBot bot (BotToken, net_ssl);
message m;

void Telegram_init()
{
    bot.begin();
    log("Telegram bot activated");
    ts.add(2, 30000, [&](void*) {
    Telegram_loop();
  }, nullptr, true);
}

int Telegram_loop()
{
  m = bot.getUpdates(); // Read new messages
  if ( m.chat_id != 0 ){ // Checks if there are some updates
    log("Telegram: " + m.text);
    sCmd.runCommand(m.text, SendTelegramMessage);
    logFlash(1, 500);
    bot.sendMessage(m.chat_id, m.text);  // Reply to the same chat with the same text
  }
}

void SendTelegramMessage(String command){
  bot.sendMessage(m.chat_id, command); 
}

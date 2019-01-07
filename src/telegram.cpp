#include <TelegramBot.h>
#include <telegram.h>

// Initialize Telegram BOT
const char BotToken[] = "456542906:AAFzslYVZIEoIwIlh6eos_dNjBVH3MSHv3s";

WiFiClientSecure net_ssl;
TelegramBot bot (BotToken, net_ssl);
message m;

void Telegram_init()
{
    bot.begin();
    logger.log("Telegram bot activated");
}

void Telegram_loop()
{
  m = bot.getUpdates(); // Read new messages
  if ( m.chat_id != 0 ){ // Checks if there are some updates
    logger.log("Telegram: " + m.text);
    sCmd.runCommand(m.text, SendTelegramMessage);
    logger.logFlash(1, 500);
    bot.sendMessage(m.chat_id, m.text);  // Reply to the same chat with the same text
  }
}

void SendTelegramMessage(String command){
  bot.sendMessage(m.chat_id, command); 
}

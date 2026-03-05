#ifndef TELEGRAM_SERVICE_H
#define TELEGRAM_SERVICE_H

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

class TelegramService {

private:
    String chatID;
    WiFiClientSecure client;
    UniversalTelegramBot* bot;

public:
    TelegramService(String token, String chatID);

    void sendMessage(String message);

    int checkMessages();

    String getMessageText(int index);

};

#endif
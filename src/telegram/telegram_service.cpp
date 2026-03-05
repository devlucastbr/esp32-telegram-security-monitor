#include "telegram_service.h"

TelegramService::TelegramService(String token, String chatID) {

    this->chatID = chatID;

    client.setInsecure();

    bot = new UniversalTelegramBot(token, client);
}

void TelegramService::sendMessage(String message) {

    bot->sendMessage(chatID, message, "");
}

int TelegramService::checkMessages() {

    int numNewMessages = bot->getUpdates(bot->last_message_received + 1);

    return numNewMessages;
}

String TelegramService::getMessageText(int index) {

    return bot->messages[index].text;
}
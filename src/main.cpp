#include <Arduino.h>
#include <WiFi.h>
#include "wifi/wifi_manager.h"
#include "telegram/telegram_service.h"
#include "motion/pir_sensor.h"
#include "secrets.h"

#define PIR_PIN 27

WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD);
TelegramService telegram(TELEGRAM_TOKEN, TELEGRAM_CHATID);
PirSensor pir(PIR_PIN);

bool systemArmed = false;

unsigned long lastAlert = 0;
unsigned long lastCommandCheck = 0;

const unsigned long alertInterval = 120000;   // 2 minutos
const unsigned long commandInterval = 3000;   // verificar comandos a cada 3s

unsigned long startTime;

String uptime() {

    unsigned long seconds = millis() / 1000;

    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;

    return String(hours) + "h " + String(minutes) + "m";
}

String wifiStatus() {

    if (WiFi.status() == WL_CONNECTED) {
        return "conectado";
    }

    return "desconectado";
}

String systemMode() {

    if (systemArmed) {
        return "Armado";
    }

    return "Desarmado";
}

void sendHelp() {

    String helpMessage =
    "🤖 *ESP32 Security Monitor*\n\n"
    "Sistema de monitoramento com sensor de movimento PIR.\n\n"

    "📜 *Comandos disponíveis:*\n"
    "/arm - Ativar monitoramento\n"
    "/disarm - Desativar monitoramento\n"
    "/status - Mostrar status do sistema\n"
    "/ping - Testar conexão com o dispositivo\n"
    "/help - Mostrar informações do sistema\n\n"

    "📡 Dispositivo: ESP32\n"
    "Sensor: PIR Motion Detector";

    telegram.sendMessage(helpMessage);
}

void handleCommands() {

    int newMessages = telegram.checkMessages();

    for (int i = 0; i < newMessages; i++) {

        String text = telegram.getMessageText(i);

        Serial.println("Comando recebido: " + text);

        // ARMAR SISTEMA
        if (text == "/arm") {

            if (!systemArmed) {

                systemArmed = true;

                telegram.sendMessage(
                "✅ Sistema armado\n"
                "Modo: Monitoramento ativo");

            } else {

                telegram.sendMessage(
                "⚠️ Sistema já está armado");

            }

        }

        // DESARMAR SISTEMA
        if (text == "/disarm") {

            if (systemArmed) {

                systemArmed = false;

                telegram.sendMessage(
                "⛔ Sistema desarmado\n"
                "Monitoramento desativado");

            } else {

                telegram.sendMessage(
                "⚠️ Sistema já está desarmado");

            }

        }

        // STATUS
        if (text == "/status") {

            String status =
            "📡 Sistema: ESP32 Security Monitor\n"
            "Status: Online\n"
            "Modo: " + systemMode() + "\n"
            "Tempo ativo: " + uptime() + "\n"
            "Sensor PIR: ativo\n"
            "WiFi: " + wifiStatus();

            telegram.sendMessage(status);

        }

        // TESTE DE CONEXÃO
        if (text == "/ping") {

            telegram.sendMessage(
            "🏓 Pong\nSistema online");

        }

        // HELP
        if (text == "/help") {

            sendHelp();

        }

    }

}

void setup() {

    Serial.begin(115200);

    Serial.println("Sistema iniciando...");

    wifiManager.connect();

    pir.begin();

    startTime = millis();

    telegram.sendMessage(
    "🔄 Sistema reiniciado\n"
    "Motivo: Após possível queda de energia\n"
    "Dispositivo: ESP32\n"
    "Status: Online\n\n"
    "Digite /help para ver os comandos disponíveis.");

}

void loop() {

    // VERIFICAR COMANDOS DO TELEGRAM
    if (millis() - lastCommandCheck > commandInterval) {

        handleCommands();

        lastCommandCheck = millis();
    }

    // DETECÇÃO DE MOVIMENTO
    if (systemArmed && pir.isMotionDetected()) {

        if (millis() - lastAlert > alertInterval) {

            telegram.sendMessage(
            "🚨 ALERTA DE MOVIMENTO\n"
            "Sensor PIR detectou atividade!");

            lastAlert = millis();

        }

    }

}
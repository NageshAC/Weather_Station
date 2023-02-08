/***************************************************************************
  
  Designed specifically to work with
  * Adafruit QT Py ESP32S2
    --> https://www.adafruit.com/product/5325
  

  Written by Nagesh Aralaguppe Channabasavanna for CGI Deutschland
 ***************************************************************************/
#pragma once

#include "Env.h"
#include <string.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define BOTtoken ""
#define chatID "" // user athentication

// time delay between 2 consecutive call
#define botDelayTime 1000


class Telebot {
  private:
  WiFiClientSecure* const _pWiFiClient;
  UniversalTelegramBot _teleBot;

  bool _debug {false};

  public:
  Telebot() = delete;
  ~Telebot() = default;

  Telebot(WiFiClientSecure* const);
  Telebot(WiFiClientSecure* const, String* const);

  bool reply(JsonObject* const);
  bool reply(const String);

};
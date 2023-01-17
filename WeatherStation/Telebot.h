/***************************************************************************
  
  Designed specifically to work with
  * Adafruit QT Py ESP32S2
    --> https://www.adafruit.com/product/5325
  

  Written by Nagesh Aralaguppe Channabasavanna for CGI Deutschland
 ***************************************************************************/
#pragma once

#include "Env.h"

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define BOTtoken ""
#define chatID ""
#define botDelayTime 1000


class Telebot {
  private:
  WiFiClientSecure _WiFiClient;
  UniversalTelegramBot _teleBot;

  bool _debug {false};

  public:
  Telebot() = delete;
  ~Telebot() = default;

};
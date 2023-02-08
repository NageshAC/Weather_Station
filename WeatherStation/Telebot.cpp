#include "Telebot.h"

Telebot::Telebot(WiFiClientSecure* const wcs)
: _pWiFiClient{wcs}, _teleBot{BOTtoken, *wcs}
{}

Telebot::Telebot(WiFiClientSecure* const wcs, String* const BotToken)
: _pWiFiClient{wcs}, _teleBot{(*BotToken).c_str(), *wcs}
{}

bool Telebot::reply(JsonObject* const json){
  String _str{""};

  for (auto keyValue : *json){
    _str += keyValue.key().c_str();
    _str += " : ";
    for(auto value : keyValue.value().as<JsonArray>()){
      _str += value.as<String>();
    }
    _str += "\n";
  }

  return reply(_str);

}

bool Telebot::reply(const String _str){
  // try:
  //   _telebot.sendMessage();
}



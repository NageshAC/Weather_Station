#include <bot.h>

Telebot::Telebot (const String botToken, const bool debug = false)
: _teleBot{botToken, _WiFiClient}, _debug{debug}
{}

void Telebot::handleNewMsg (const int newMsgNum) {
  if (_debug) {
    Serial.print("Handling Message: ");
    Serial.println(newMsgNum);
  } 

  // If the type is a "callback_query", a inline keyboard button was pressed
  if (bot.messages[i].type ==  F("callback_query")) {

      String text = bot.messages[i].text;
      if (_debug) {
        Serial.print("Call back button pressed with text: ");
        Serial.println(text);
      }

      if (text == F("update")) {
        
      }
  }
  
  
}

void checkNewMsg() {
  if (_debug) Serial.println ("Checking for new Msgs");

  // run loop until queries are done
  int newMsg {_teleBot.getUpdates(_teleBot.last_message_recieved + 1)};
  while (newMsg) {
    handleNewMsg(newMsg);
    newMsg = _teleBot.getUpdates(_teleBot.last_message_recieved + 1);
  }

}
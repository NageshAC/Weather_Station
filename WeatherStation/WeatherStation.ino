#define DEBUG true // in debug mode if true

#include<Wire.h>
#define I2C_STEMMAQT_WIRE &Wire1

// // Setting up Web DataBase -> Google SpreadSheet
#include <WiFiClientSecure.h>
const String DepID = "AKfycbwPgVNzDQfcan2mkQPyjfYY7umdawW2N33WfGe94fdU4zqTVy6TBewcO9Iu69gx5vCfew";
const String host = "script.google.com";
const uint16_t HTTPSport = 443;
String QueryURL = "/macros/s/" + DepID + "/exec";
WiFiClientSecure wcs{};


// // WiFi AP credentials
#include <WiFiManager.h>
#define WiFiAPname "CGI_AP"
#define WiFiAPpass "cgi@cgi.com"

// // Preparing NeoPixel LED
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel pix (1, PIN_NEOPIXEL);
#include "LED.h"
LED WiFiLED(&pix, 'B');
LED ErrLED(&pix, 'R');
LED BusyLED(&pix, 'G');

// // Preparing the sensors
#include "Env.h"
Adafruit_BME680 bme(I2C_STEMMAQT_WIRE);
Adafruit_LTR390 ltr {Adafruit_LTR390()};
Env env{&ErrLED, &BusyLED, &bme, &ltr, &wcs, host, QueryURL, HTTPSport, I2C_STEMMAQT_WIRE, DEBUG};


// // Telegram Bot setup
// #include "Telebot.h"

void setup() {

  // // Serial Port initialisation
  Serial.begin(115200);
  
  // // WiFi AP setup
  WiFi.mode(WIFI_STA);
  WiFiManager wm{};
  // if (DEBUG) wm.resetSettings(); // Not required for final production
  WiFiLED.on();
  if (wm.autoConnect(WiFiAPname, WiFiAPpass)){
    WiFiLED.off();
    if (DEBUG)
    Serial.println("WiFi Connected");
  }
  else {
    ErrLED.on();
    if (DEBUG)
      Serial.println("WiFi could not be Connected");
  }
  wcs.setInsecure();

  // // LED initialisation
  pix.begin();

  // // Environment sensor
  env.init();
  

}

void loop() {
  env.update();
  delay(UPDATE_TIME);
}

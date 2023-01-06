#define DEBUG true // in debug mode if true

#include<Wire.h>
#define I2C_STEMMAQT_WIRE &Wire1

// // WiFi AP credentials
// #include <WiFiManager.h>
// #define WiFiAPname "CGI_AP"
// #define WiFiAPpass "cgi@cgi.com"

// // Preparing NeoPixel LED
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel pix (1, PIN_NEOPIXEL);
#include "LED.h"
LED WiFiLED(&pix, 'B');
LED ErrLED(&pix, 'R');
LED BusyLED(&pix, 'G');

// Preparing the sensors
// #include <Adafruit_BME680.h>
#include "Env.h"
Adafruit_BME680 bme(I2C_STEMMAQT_WIRE);
// Adafruit_LTR390 ltr(I2C_STEMMAQT_WIRE);
Env env{&ErrLED, &BusyLED, &bme, DEBUG};

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println("Adafruit BME-680 test");
  pix.begin();
  env.init();

  // // WiFi AP setup
  // WiFi.mode(WIFI_STA);
  // WiFiManager wm{};
  // if (DEBUG)
  // wm.resetSettings(); // Not required for final production
  // WiFiLED.on();
  // if (wm.autoConnect(WiFiAPname,WiFiAPpass)){
  //   WiFiLED.off();
  //   if (DEBUG)
  //   Serial.println("WiFi Connected");
  // }else{
  //   ErrLED.on();
  //   if (DEBUG)
  //   Serial.println("WiFi could not be Connected");
  // }

  // // Environment sensor
  

}

void loop() {
  env.log();
  delay(3000);
}

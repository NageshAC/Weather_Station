/***************************************************************************
  
  Designed specifically to work with
  * Adafruit BME680 Breakout 
    --> http://www.adafruit.com/products/3660
  * Adafruit LTR390 UV Light Sensor Breakout 
    --> https://www.adafruit.com/product/4831
  

  Written by Nagesh Aralaguppe Channabasavanna for CGI Deutschland
 ***************************************************************************/
#pragma once
#include <stdint.h>
#include "esp32-hal.h" // for millis() func
#include <Adafruit_BME680.h>
#include <Adafruit_LTR390.h>
#include <ArduinoJson.h>
#include "LED.h"

#define P_SEALVL (1013.25) // Pressure at sea level (in mBar)

typedef unsigned long int ul_t;

class Env {

  private:

  LED * const _ErrLED{nullptr}; // used to indicate error
  LED * const _BusyLED{nullptr}; // used to indicate that the microcontroler is busy
  Adafruit_BME680* const _pbme{nullptr}; // pointer to Adafruit BME-680 device variable
  Adafruit_LTR390* const _pltr{nullptr}; // pointer to Adafruit LTR-390 device variable

  uint32_t  _time{},   // time for the current reading
            _wtime{};  // wait time for reading to complete

  bool  _debug{false},
        _bme{false},  // check if BME-680 is connected
        _ltr{false},  // check if LTR390(UV) is connected
        _init{false}; // check if Env is initialised

  StaticJsonDocument<500> _out{}; // JSON variable for reporting output
  String _sout{}; // String variable for pretified JSON

  public:
  Env () = delete;
  ~Env () = default;

  Env (LED* const, LED* const,  Adafruit_BME680* const, Adafruit_LTR390* const, const bool);
  Env (LED* const, LED* const,  Adafruit_BME680* const, const bool);
  void init ();
  void check_connection ();
  void update ();
  const String output_json ();
  void log ();
};

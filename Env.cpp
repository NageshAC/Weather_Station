#include "USBCDC.h"
#include <stdint.h>
#include "esp32-hal.h"
#include "Env.h"

Env::Env(LED* const ErrLED, LED* const BusyLED, Adafruit_BME680* const bme, Adafruit_LTR390* const ltr, const bool debug=false)
: _ErrLED{ErrLED}, _BusyLED{BusyLED}, _pbme{bme}, _pltr{ltr}, _debug{debug}
{}

Env::Env(LED* const ErrLED, LED* const BusyLED, Adafruit_BME680* const bme, const bool debug=false)
: _ErrLED{ErrLED}, _BusyLED{BusyLED}, _pbme{bme}, _debug{debug}
{}

void Env::init(){
  if (_debug) Serial.println("Setting up Environment sensor............");

  _time = millis();

  if (_pbme) _bme = _pbme -> begin();
  if (_pltr) _ltr = _pltr -> begin();

  check_connection();

  if (_bme) {
    _pbme -> setTemperatureOversampling (BME68X_OS_16X);
    _pbme -> setPressureOversampling    (BME68X_OS_16X);
    _pbme -> setHumidityOversampling    (BME68X_OS_16X);
    _pbme -> setIIRFilterSize           (BME680_FILTER_SIZE_3);
    _pbme -> setGasHeater               (320, 150); // 320*C for 150 ms
  }

  _out["time"] = _time;
  _out["BME680 status: "] = _bme;
  _out["LTR390 Status: "] = _ltr;
  JsonArray data = _out.createNestedArray("T");
  data.add(0.);
  data.add(" *C");
  data = _out.createNestedArray("H");
  data.add(0.);
  data.add(" %");
  data = _out.createNestedArray("P");
  data.add(0);
  data.add(" mBar");
  data = _out.createNestedArray("G");
  data.add(0);
  data.add(" kOhms");
  data = _out.createNestedArray("UV");
  data.add(0);
  data.add(" ");

  _init = true;
}

void Env::check_connection () {
  // check if BME-680 is connected
  if (_pbme) _bme = _pbme -> begin();
  if (!_bme)
    Serial.println("Could not find a valid BME-680 sensor, check wiring!");
  else
    Serial.println("BME680 found");
  
  // check if LTR-390 is connected
  if (_pltr) _ltr = _pltr -> begin();
  if (!_ltr)
    Serial.println("Could not find a valid LTR-390 sensor, check wiring!");
  else
    Serial.println("LTR-390 found");
}

void Env::update () {
  // check_connection();
  if ( millis()-_time > 60000 || _time < 60000 ) {
    if (_bme) {
      _BusyLED -> on();
      uint32_t _start{millis()};
      Serial.printf("Start time:  %d \n", &_start);
      // _out["BME680 status: "] = _bme;
      // _out["LTR390 Status: "] = _ltr;
      _out["T"][0] = _pbme -> readTemperature();
      _out["H"][0] = _pbme -> readHumidity();
      _out["P"][0] = _pbme -> readPressure() / 100.;
      _out["G"][0] = _pbme -> readGas() / 1000.;
      uint32_t _end{millis()};
      Serial.printf("End time:    %d \n", &_end);
      _wtime = _end-_start;
      Serial.printf("Diff time:   %d \n", &_wtime);

      _wtime = _BusyLED -> off();
      if (_debug) Serial.printf("Wait time: %d \n", &_wtime);
    }
  }
}

const String Env::output_json() {
  update();
  _sout = "";
  serializeJsonPretty (_out, _sout);
  return _sout;
}

void Env::log (){
  Serial.println(output_json());
}
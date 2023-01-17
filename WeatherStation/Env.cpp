#include "Env.h"

Env::Env(LED* const ErrLED, LED* const BusyLED, Adafruit_BME680* const bme, const bool debug=false)
: _ErrLED{ErrLED}, _BusyLED{BusyLED}, _pbme{bme}, _debug{debug}
{}

Env::Env(LED* const ErrLED, LED* const BusyLED, Adafruit_BME680* const bme, Adafruit_LTR390* const ltr, TwoWire* const theWire, const bool debug=false)
: _ErrLED{ErrLED}, _BusyLED{BusyLED}, _pbme{bme}, _pltr{ltr}, _Wire{theWire}, _debug{debug}
{}

Env::Env(LED* const ErrLED, LED* const BusyLED, Adafruit_BME680* const bme, Adafruit_LTR390* const ltr, WiFiClientSecure* const wifi, const String host, const String Qurl, const uint16_t HTTPSport, TwoWire* const theWire, const bool debug=false)
: _ErrLED{ErrLED}, _BusyLED{BusyLED}, _pbme{bme}, _pltr{ltr}, _pwcs{wifi}, _host{host}, _Qurl{Qurl}, _port{HTTPSport}, _Wire{theWire}, _debug{debug}
{}

void Env::init(){
  if (_debug) Serial.println("Setting up Environment sensor............");

  _time = millis();

  check_connection();

  if (_bme) {
    _pbme -> setTemperatureOversampling (BME68X_OS_16X);
    _pbme -> setPressureOversampling    (BME68X_OS_16X);
    _pbme -> setHumidityOversampling    (BME68X_OS_16X);
    _pbme -> setIIRFilterSize           (BME680_FILTER_SIZE_3);
    _pbme -> setGasHeater               (320, 150); // 320*C for 150 ms
  }

  if (_ltr) {
    _pltr -> setMode          (LTR390_MODE_UVS);
    _pltr -> setGain          (LTR390_GAIN_3);
    _pltr -> setResolution    (LTR390_RESOLUTION_16BIT);
    _pltr -> setThresholds    (100, 1000);
    _pltr -> configInterrupt  (true, LTR390_MODE_UVS);
  }

  // _out["time"] = _time;
  // _out["BME680 status: "] = _bme;
  // _out["LTR390 Status: "] = _ltr;
  JsonArray data = _out.createNestedArray("T");
  data.add(0.);
  data.add(" *C");
  data = _out.createNestedArray("H");
  data.add(0.);
  data.add(" %");
  data = _out.createNestedArray("P");
  data.add(0.);
  data.add(" mBar");
  data = _out.createNestedArray("G");
  data.add(0.);
  data.add(" kOhms");
  data = _out.createNestedArray("UV");
  data.add(0.);
  data.add(" ");
  data = _out.createNestedArray("WS");
  data.add(0.);
  data.add(" rpm");
  data = _out.createNestedArray("WD");
  data.add(0.);
  data.add(_sdir[_dir]);

  _init = true;

  update();

}

void Env::check_connection () {
  /*!
      Args: None
      @brief  Sets _bme and _ltr to true if BME-680 and LTR-390 are connected
  */

  // check if BME-680 is connected
  if (_pbme) _bme = _pbme -> begin();
  if (!_bme)
    Serial.println("Could not find a valid BME-680 sensor, check wiring!");
  else
    Serial.println("BME680 found");
  
  // check if LTR-390 is connected
  if (_pltr) _ltr = _pltr -> begin(_Wire);
  if (!_ltr)
    Serial.println("Could not find a valid LTR-390 sensor, check wiring!");
  else
    Serial.println("LTR-390 found");
}

void Env::update () {
  // check_connection();

  if (!_init) init();

  _BusyLED -> on();

  if ( millis()-_time > UPDATE_TIME ) {
    if (_bme) {
      // _out["BME680 status: "] = _bme;
      // _out["LTR390 Status: "] = _ltr;

      _out["T"][0]  = _pbme -> readTemperature();
      _out["H"][0]  = _pbme -> readHumidity();
      _out["P"][0]  = _pbme -> readPressure() / 100.;
      _out["G"][0]  = _pbme -> readGas() / 1000.;
    }
    if (_ltr) {
      // if (_debug) Serial.println(_pltr -> readUVS());
      _out["UV"][0] = _pltr -> readUVS();
    }
  }

  if (_pwcs) {
    update_DB();
  }

  _wtime = _BusyLED -> off();
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

void Env::update_DB() {
  if (_debug) {
    Serial.println("==========");
    Serial.print("connecting to ");
    Serial.println(_host);
  }
  // Connect to host
  if (!_pwcs -> connect(_host.c_str(), _port) && _debug) {
    Serial.println("connection failed");
    return;
  }
  else if (_debug){
    Serial.println("Successfully Connected");
  }

  // Processing data and sending data
  String url = _Qurl + "?";
  for (const auto& kv : _out.as<JsonObject>()){
    url += "&" ;
    url += kv.key().c_str();
    url += "=";
    url += kv.value()[0].as<String>();
  }
  if (_debug) {
    Serial.print("requesting URL: ");
    Serial.println(url);
  }

  _pwcs -> print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + _host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  if (_debug) Serial.println("request sent");

  while (_pwcs -> connected()) {
    String line = _pwcs -> readStringUntil('\n');
    if (line == "\r") {
      if (_debug) Serial.println("Headers Received Sucessfully");
      break;
    }
  }
  if (_debug) {
    String line = _pwcs -> readStringUntil('\n');
    Serial.print("reply was : ");
    Serial.println(line);
    Serial.println("closing connection");
    Serial.println("==========");
    Serial.println();
  }

}
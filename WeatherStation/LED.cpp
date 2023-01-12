#include <stdint.h>
#include "esp32-hal.h"
#include "LED.h"

LED::LED (Adafruit_NeoPixel* pix, const char clr)
: _pixel{pix}, _color{clr}
{}

void LED::on() {
  _start = millis();
  switch ((char)toupper(_color)) {
    case 'R': _pixel->setPixelColor(0, _pixel->Color(255,0,0)); break;
    case 'G': _pixel->setPixelColor(0, _pixel->Color(0,255,0)); break;
    case 'B': _pixel->setPixelColor(0, _pixel->Color(0,0,255)); break;
    default: break;
  }
  _pixel->show();
}

uint32_t LED::off() const {
  _pixel->clear();
  _pixel->show();
  return (millis()-_start);
}

void LED::blink (const long Ton = 1000, const long Toff = 1000) {
  on();
  delay(Ton);
  off();
  delay(Toff);
}

void LED::set_color (const char clr) { _color = clr; }

const char LED::get_color () const { return _color; }

const bool LED::status() const { return _on; }
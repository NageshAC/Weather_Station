/***************************************************************************
 ***************************************************************************/

#pragma once
#include <Adafruit_NeoPixel.h>

class LED{
  private:
  Adafruit_NeoPixel* const _pixel{nullptr};
  char _color{};
  bool _on{false};
  uint32_t _start{};

  public:
  LED () = delete;
  ~LED () = default;

  LED (Adafruit_NeoPixel*, const char);
  void on ();
  uint32_t off () const;
  void blink (const long, const long );
  void set_color (const char);
  const char get_color () const;
  const bool status () const;

};
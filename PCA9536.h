/*
  Copyright 2017 Chiehting Huang <j.huang.1985@gmail.com>

*/
#ifndef PCA9536_h
#define PCA9536_h

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#ifdef __AVR_ATtiny85__
 #include "TinyWireM.h"
 #define Wire TinyWireM
#elif defined(ARDUINO_ARCH_ESP8266)
 #include <Wire.h>
#else
 #include <avr/pgmspace.h>
 #include <Wire.h>
#endif

class PCA9536
{
  public:
    PCA9536();
#if defined(ARDUINO_ARCH_ESP8266)
    void    begin(int SDA_pin, int SCL_pin);
#else
    void    begin();
#endif
    bool    init();
    bool    exist();

    void    pinMode(uint8_t pin, uint8_t direction);
    void    digitalWrite(uint8_t pin, uint8_t level);
    uint8_t digitalRead(uint8_t pin); 
  
    uint8_t input();
    uint8_t readOutput();
    void    output(uint8_t value);
    void    polarity(uint8_t value);  // 1: invert, 0: normal
    void    direction(uint8_t value); // 1: input,  0: output
    
  private:
    const uint8_t address = 0x41;

    struct State
    {
      uint32_t input     : 4;
      uint32_t output    : 4;
      uint32_t direction : 4;
      uint32_t polarity  : 4;
      uint32_t init      : 1;
      uint32_t reserve   : 15;
    };

    State m_state;

    void    write8(uint8_t addr, uint8_t data);
    uint8_t read8(uint8_t addr);
};

#endif


/*
    Copyright 2017 Chiehting Huang <j.huang.1985@gmail.com>
*/
#include "Arduino.h"
#include "PCA9536.h"

// I2C commands
#define INPUT_REG    0x00
#define OUTPUT_REG   0x01
#define POLARITY_REG 0x02
#define CONFIG_REG   0x03

PCA9536::PCA9536()
{
  m_state.output    = 0xFF;
  m_state.input     = 0xFF;
  m_state.direction = 0xFF;
  m_state.polarity  = 0x00;
}

#if defined(ARDUINO_ARCH_ESP8266)
void PCA9536::begin(int SDA_pin, int SCL_pin) {
    Wire.begin(SDA_pin,SCL_pin);
#else
bool PCA9536::begin() {
    Wire.begin();
#endif
}

bool PCA9536::init()
{
  if (exist() == true)
  {
    polarity(m_state.polarity);
    direction(m_state.direction);
    output(m_state.output);
    return true;
  }
  return false;
}

bool PCA9536::exist() 
{
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
        return true;
    }
    return false;
}

void PCA9536::pinMode(uint8_t pin, uint8_t direct)
{
  if (pin < 4)
  {
    uint8_t mask = 1 << pin;
    if (direct == OUTPUT)
    {
      m_state.direction &= (~mask);
    }
    else if (direct == INPUT)
    {
      m_state.direction |= (mask);
    }
    
    direction(m_state.direction);
  }
}
void PCA9536::digitalWrite(uint8_t pin, uint8_t level)
{
  if (pin < 4)
  {
    uint8_t mask = 1 << pin;
    if (level == LOW)
    {
      m_state.output &= (~mask);
    }
    else if (level == HIGH)
    {
      m_state.output |= (mask);
    }
    
    output(m_state.output);
  }
}

uint8_t PCA9536::digitalRead(uint8_t pin)
{
  if (pin < 4)
  {
    uint8_t mask = 1 << pin;
    uint8_t v = input();
    if (v & mask)
    {
      return HIGH;
    }
  }
  return LOW;
}

uint8_t PCA9536::input() {
    return read8(INPUT_REG);
}

uint8_t PCA9536::readOutput() {
    return read8(OUTPUT_REG);
}

void PCA9536::output(uint8_t value) {
    return write8(OUTPUT_REG, value);
}

void PCA9536::polarity(uint8_t value) {
    return write8(POLARITY_REG, value);
}

void PCA9536::direction(uint8_t value) {
    return write8(CONFIG_REG, value);
}


void PCA9536::write8(uint8_t addr, uint8_t data) {
    Wire.beginTransmission(address); // start transmission to device 
#if (ARDUINO >= 100)
    Wire.write(addr); // sends register address to read from
    Wire.write(data);  // write data
#else
    Wire.send(addr); // sends register address to read from
    Wire.send(data);  // write data
#endif
    Wire.endTransmission();
}

uint8_t PCA9536::read8(uint8_t addr) {
    uint8_t data;
	
	  Wire.beginTransmission(address);

#if ARDUINO >= 100
    Wire.write(addr);
#else
    Wire.send(addr);
#endif

	  Wire.endTransmission();
		Wire.requestFrom(address, (uint8_t) 1);
		

#if ARDUINO >= 100
    data = Wire.read();
#else
    data = Wire.receive();
#endif

	  return data;
}

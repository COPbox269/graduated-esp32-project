#include "Arduino.h"
#include "MyButton.h"

/**
 * 
 * Should not use for interrupt
 */ 
MyButton::MyButton(uint8_t pin, uint16_t inputMode, uint8_t isPressedLevel) {
  this->pin = pin;
  this->inputMode = inputMode;
  this->isPressedLevel = isPressedLevel;

  init();
}

/**
 * Contructor use for interrupt
 */ 
MyButton::MyButton(uint8_t pin, uint16_t inputMode) {
  this->pin = pin;
  this->inputMode = inputMode;

  init();
}

void MyButton::init() {
  pinMode(pin, inputMode);
}

/**
 * Read current state of button pin
 */ 
bool MyButton::readDigital() {
  return digitalRead(pin);
}

// Note use for interrupt
bool MyButton::status() {
  uint8_t val = readDigital();
  bool isPressed;

  if(val == isPressedLevel) {
    isPressed = true;
  }
  else {
    isPressed = false;
  }

  return isPressed;
}

void MyButton::setInterrupt(void ISR(), int mode) {
  attachInterrupt(pin, ISR, mode);
}

void MyButton::stopInterrupt() {
  detachInterrupt(pin);
}
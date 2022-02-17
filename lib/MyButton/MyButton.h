#ifndef MyButton_h
#define MyButton_h
#endif

#include "Arduino.h"

class MyButton {
  public:
    // The Flag notifies the interrupt have been occured
    volatile bool FLAG_ISPRESSED = false;

    MyButton(uint8_t pin, uint16_t inputMode, uint8_t isPressedLevel);

    MyButton(uint8_t pin, uint16_t inputMode);

    void init();

    bool status();

    bool readDigital();

    void setInterrupt(void ISR(), int mode);

    void stopInterrupt();

  private:
    uint8_t pin;

    // bool isPressed;
    uint8_t isPressedLevel;

    uint16_t inputMode;
};
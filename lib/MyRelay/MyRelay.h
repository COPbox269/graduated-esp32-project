#ifndef MyRelay_h
#define MyRelay_h
#endif

#include "Arduino.h"

#define LOW_LEVEL_TRIGGER 0
#define HIGH_LEVEL_TRIGGER  1

typedef bool state;
const state ON = true;
const state OFF = false;

class MyRelay {
  public:
    static uint8_t STATE_ADDRESS_EEPROM;

    MyRelay(uint8_t pin, uint8_t levelTrigger, uint8_t positionInEEPROM);

    MyRelay(uint8_t pin, uint8_t levelTrigger);

    MyRelay(uint8_t pin);

    void init();
    
    void on();

    void off();

    void toggle();

    void setState(state state);

    state getState();

    void saveStateToEEPROM();

    bool readStateFromEEPROM();

  private:
    uint8_t pin;

    // Define level trigger of relay module
    // - LOW : get low (0) logic level to trigger
    // - HIGH : get high (1) logic level to trigger
    uint8_t levelTrigger;

    // This variable is compatible with real state of relay
    // - true if relay is ON
    // - false if relay is OFF
    volatile state relayStatus = false;

    // unique position of relay state in "STATE_ADRESS_EEPROM" (must be in: 0->7)
    uint8_t positionInEEPROM;

    uint8_t getBitFromByte(int byte, uint8_t bitPosition);
};
#include "Arduino.h"
#include "MyRelay.h"
#include <EEPROM.h>

// This address provide 8-bit to save 8 relay states
// The default address is 0. It can change but it must be unique address
// 0 -> 511 (512 memory stack)
uint8_t MyRelay::STATE_ADDRESS_EEPROM = 0;

/**
 * @param pin pin of relay
 * @param levelTrigger indicate logic level to trigger relay state
 * @param positionInEEPROM position of each relay state in STATE_ADDRESS_EEPROM (default is 0)
 */ 
MyRelay::MyRelay(uint8_t pin, uint8_t levelTrigger, uint8_t positionInEEPROM) {
    this->pin = pin;
    this->levelTrigger = levelTrigger;
    this->positionInEEPROM = positionInEEPROM;

    init();
}

/**
 * @param pin pin of relay
 * @param levelTrigger indicate logic level to trigger relay state
 */ 
MyRelay::MyRelay(uint8_t pin, uint8_t levelTrigger) {
    this->pin = pin;
    this->levelTrigger = levelTrigger;

    init();
}

/**
 * @param pin pin of relay
 * The default levelTrigger is HIGH
 * Recommend using another contructor
 */ 
MyRelay::MyRelay(uint8_t pin) {
    this->pin = pin;

    // default levelTrigger
    this->levelTrigger = HIGH;

    init();
}

void MyRelay::init() {
    pinMode(pin, OUTPUT);
    // Sync state of this pin in EEPROM if necessary
    off();
}

void MyRelay::on() {
    relayStatus = ON;

    digitalWrite(pin, levelTrigger);
}

void MyRelay::off() {
    relayStatus = OFF;

    digitalWrite(pin, !levelTrigger);
}

void MyRelay::toggle() {
    relayStatus = !relayStatus;

    if(relayStatus == ON) {    // relay will ON
        digitalWrite(pin, levelTrigger);
    }
    else if(relayStatus == OFF) {  // relay will OFF
        digitalWrite(pin, !levelTrigger);
    }
}

/**
 * Set state to relay. This makes the relay ON/OFF
 * @param state state of relay
 */ 
void MyRelay::setState(state relayStatus) {
    if(relayStatus == ON) {
        on();
    }
    else if(relayStatus == OFF) {
        off();
    }
}

/**
 * Get value of this relay pin (HIGH-true, LOW-false)
 * @return relayStatus
 */ 
state MyRelay::getState() {
    return relayStatus;
}

void MyRelay::saveStateToEEPROM() {
    // see here: https://www.geeksforgeeks.org/set-clear-and-toggle-a-given-bit-of-a-number-in-c/
    int val = EEPROM.read(STATE_ADDRESS_EEPROM);

    if(relayStatus == ON) {
        // TODO set bit
        val = val | (1 << positionInEEPROM);
    }
    else {
        // TODO clear bit
        val = val & ~(1 << positionInEEPROM);
    }

    EEPROM.write(STATE_ADDRESS_EEPROM, val);
    EEPROM.commit();
}

bool MyRelay::readStateFromEEPROM() {
    int val = EEPROM.readInt(STATE_ADDRESS_EEPROM);

    relayStatus = getBitFromByte(val, positionInEEPROM);

    return relayStatus;
}

/**
 * @param byte: bit sequence
 * @param poisitionBit: position of specific bit in "b"
 * 
 * @return value of bit (1-true, 0-false)
 */ 
uint8_t MyRelay::getBitFromByte(int byte, uint8_t positionBit) {
    // see here: https://stackoverflow.com/questions/4854207/get-a-specific-bit-from-byte/4854257
    uint8_t bit = (byte & (1 << positionBit)) != 0;

    return bit;
}
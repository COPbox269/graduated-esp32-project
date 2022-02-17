#ifndef MyPZEM004T_h
#define MyPZEM004T_h
#endif

#include "Arduino.h"

class MyPZEM004T {
  public:

    MyPZEM004T();

    ~MyPZEM004T();

    void init();
    
    float readVoltage();

    float readCurrent();

    float readPower();

    float readEnergy();

    float readFrequency();

    float readPowerFactor();

    float voltage;

    float current;

    float power;

    float energy;

    float frequency;

    float powerFactor;

    const unsigned long TIME_MS_READ_DATA_DELAY = 3000;

    unsigned long time_ms_previous_read_data = 0;
};
#include <Arduino.h>
#include <WiFi.h>

#include "MyPZEM004T.h"
#include <PZEM004Tv30.h>

/* Hardware Serial3 is only available on certain boards.
* For example the Arduino MEGA 2560
* NOTE: Disable "SoftwareSerial" in PZEM004Tv30.cpp (library)
*/  
PZEM004Tv30 pzem(&Serial2);

MyPZEM004T::MyPZEM004T() {
    init();
}

MyPZEM004T::~MyPZEM004T() {}

void MyPZEM004T::init() {
//   Serial2.begin(9600, SERIAL_8N1, 16, 17);
}

float MyPZEM004T::readVoltage() {
    float voltage = pzem.voltage();

    if(!isnan(voltage)){
        this->voltage = voltage;

        // Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
    } else {
        // Serial.println("Error reading voltage");
        voltage = 0;
    }

    return voltage;
}

float MyPZEM004T::readCurrent() {
    float current = pzem.current();

    // if(!isnan(current)){
        if(!isnan(current)){
        this->current = current;

        // Serial.print("Current: "); Serial.print(current); Serial.println("A");
    } else {
        // Serial.println("Error reading current");
        current = 0;
    }

    return current;
}

float MyPZEM004T::readPower() {
    float power = pzem.power();

    if(!isnan(power)){
        this->power = power;

        // Serial.print("Power: "); Serial.print(power); Serial.println("W");
    } else {
        // Serial.println("Error reading power");
        power = 0;
    }

    return power;
}

float MyPZEM004T::readEnergy() {
    float energy = pzem.energy();

    if(!isnan(energy)){
        this->energy = energy; 

        // Serial.print("Energy: "); Serial.print(energy,3); Serial.println("kWh");
    } else {
        // Serial.println("Error reading energy");
        energy = 0;
    }

    return energy;
}

float MyPZEM004T::readFrequency() {
    float frequency = pzem.frequency();

    if(!isnan(frequency)){
        this->frequency = frequency;

        // Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz");
    } else {
        // Serial.println("Error reading frequency");
        frequency = -1;
    }

    return frequency;
}

float MyPZEM004T::readPowerFactor() {
    float powerFactor = pzem.pf();

    if(!isnan(powerFactor)){
        this->powerFactor = powerFactor;

        Serial.print("PF: "); Serial.println(powerFactor);
    } else {
        Serial.println("Error reading power factor");
        powerFactor = -1;
    }

    return powerFactor;
}
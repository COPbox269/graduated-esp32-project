#ifndef MyLed_h
#define MyLed_h
#endif

#include "Arduino.h"

class MyLed : public MyRelay
{
    public:
        // inherit all contructor of base (parent) class
        using MyRelay::MyRelay;

        bool FLAG_EXIST_BLINK = false;

        void setBlinkTimer(int chanel, float interval, void onTimer(), uint16_t loops);

        void blink();

        void stopBlinkTimer();

    private:
        // loops <= 0 : infinite loop
        uint16_t loops;

        uint16_t countInterrupt;
};
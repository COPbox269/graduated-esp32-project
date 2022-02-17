#include "Arduino.h"
#include "MyRelay.h"
#include "MyLed.h"

hw_timer_t *timerLed = NULL;

void MyLed::blink()
{
    MyLed::toggle(); // or toogle()

    if (loops > 0) {
        if (countInterrupt <= loops) {
            countInterrupt = countInterrupt + 1;
        }
        else {
            // Stop and free timer, turn off LED
            if (timerLed) {
                off();

                stopBlinkTimer();
            }
        }
    }
}

void MyLed::stopBlinkTimer() {
    timerEnd(timerLed);
    timerLed = NULL;

    countInterrupt = 0;
}

/**
 * @param chanel 4 timer (0-3)
 * @param interval repeating time
 * @param onTimer() the function is called when timer trigger
 * @param loops number of repeation (if <= 0 : infinity loops) 
 */
void MyLed::setBlinkTimer(int chanel, float interval, void onTimer(), uint16_t loops) {
    // set prescaler
    timerLed = timerBegin(chanel, 80, true);
    // set interrupt funtion
    timerAttachInterrupt(timerLed, onTimer, true);
    // set time (ms)
    interval = interval / 1000 * 1000000; // ms
    timerAlarmWrite(timerLed, interval, true);
    // enable alarm
    timerAlarmEnable(timerLed);

    this->loops = loops;

    digitalWrite(LED_BUILTIN, HIGH);
}
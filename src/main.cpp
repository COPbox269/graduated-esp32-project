/*
#02:20, Mon/15/02/2021: Initial

  HARDWARE
    + NodeMCU-32s
    + LED status
    + Relay output
    + Button(if need)
    + Voltage measurement sensor
    + Current measurement sensor

  NodeMCU-32s Programming 
    + LED
    + EEPROM
    + Webserver
    + WiFi
    + Firebase
    + Interrupt

  NodeMCU32s's Webserver: WiFi Configuration
    + Check connection status (use interrupt)
    + Scan network
    + Connect to network (save in EEPROM to connect next time)
    + Change WiFi AP and WiFi Station of device

  Android Studio
    + Display (power, connection status)
    + Change WiFi Staion
    + Control
      # virtual keyboard in app
      # google voice input

#07:30, Tues, 19/05/2021: Change FLAG_SYNC_DATA_FIREBASE
#6:46pm Saturday, 03/07/2021 - clear delay() at readWiFiFromEEPROM()
#05/07/2021 - Cant put webserver.handleClient() in timer intterrupt
#06/07/2021 - Change main
*/

#include <Arduino.h>
#include <string.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <EEPROM.h>
#include <ESP32Ping.h>

#include "MyWiFi.h"
#include "MyWiFiManager.h"
#include "MyWebServer.h"
#include "MyFirebase.h"
#include "MyRelay.h"
#include "MyButton.h"
#include "MyLed.h"
#include "MyPZEM004T.h"

#define FIREBASE_HOST "https://nodemcu-32-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "XXRsclzpZYZE01FroXfiajHJmASp2SxQFMAR6n0D"

#define BUTTON1_PIN 18
#define BUTTON2_PIN 19
#define BUTTON3_PIN 21
#define BUTTON4_PIN 22

#define SWITCH_PIN 4

#define RELAY1_PIN 26
#define RELAY2_PIN 25
#define RELAY3_PIN 33
#define RELAY4_PIN 32

int mode = 0;   // 0: disial network, 1: enable network

MyWiFi mSavedWiFi;

MyPZEM004T mPZEM004T;

MyLed mLedWiFiConnection(LED_BUILTIN, HIGH);

volatile bool FLAG_OCCUR_BUTTON_INTERRUPT = false;
MyButton mButton1(BUTTON1_PIN, INPUT_PULLDOWN);
MyButton mButton2(BUTTON2_PIN, INPUT_PULLDOWN);
MyButton mButton3(BUTTON3_PIN, INPUT_PULLDOWN);
MyButton mButton4(BUTTON4_PIN, INPUT_PULLDOWN);

unsigned long time_ms_previous_pressed_switch;
volatile bool FLAG_OCCUR_SWITCH_INTERRUPT = false;
bool FLAG_SWITCH = false;
bool FLAG_CHANGE_MODE = false;
MyButton mSwitch(SWITCH_PIN, INPUT_PULLDOWN);

MyRelay mRelay1(RELAY1_PIN, LOW, 0);
MyRelay mRelay2(RELAY2_PIN, LOW, 1);
MyRelay mRelay3(RELAY3_PIN, LOW, 2);
MyRelay mRelay4(RELAY4_PIN, LOW, 3);

MyFirebase mFirebaseControl;
volatile bool FLAG_FIREBASE_CONTROL_SETTER = false;
volatile bool FLAG_FIREBASE_STREAM_TRIGGER = false;
String childPath[4] = {"/0", "/1", "/2", "/3"}; // rleative to relay1, relay2, relay3, relay4
size_t numChild = sizeof(childPath) / sizeof(childPath[0]);

MyFirebase mFirebaseParameter;

void handleRelayStatus();

void restartEsp() {
    ESP.restart();
}

bool checkInternet() {
    if(mode == 1) {
        bool ret = Ping.ping("www.google.com");

        if(ret) {
            Serial.println("Have WiFi Connection");

            MyWiFiManager.internetStatus = true;

            mLedWiFiConnection.on();
        }
        else {
            Serial.println("Lost WiFi Connection!!!");

            MyWiFiManager.internetStatus = false;
        }
    return ret;
    }
    else {
        return false;
    }
}

/*--- Stream Callback Block ---*/
// Global function that handles stream data
// NOTE: stream callback is only called when particular child path updating
void streamCallback(MultiPathStreamData stream)
{
    Serial.println();
    Serial.println("Stream Control Data Getter available...");

    if (FLAG_FIREBASE_CONTROL_SETTER)
    {
        Serial.println("Block read");
        FLAG_FIREBASE_CONTROL_SETTER = false;
        return;
    }

    // Make sure block read          
    for (size_t i = 0; i < numChild; i++) {
        if (stream.get(childPath[i]))
        {
            Serial.println("path: " + stream.dataPath + ", type: " + stream.type + ", value: " + stream.value);

            String childName = stream.dataPath;
            String dataType = stream.type;
            bool val = stream.value.equals("true") ? true : false;

            if (dataType.equals("boolean"))
            { // make sure to get data have type is bool
                // map to state of relay
                if (childName.equals("/0"))
                {
                    mRelay1.setState(val);
                }
                else if (childName.equals("/1"))
                {
                    mRelay2.setState(val);
                }
                else if (childName.equals("/2"))
                {
                    mRelay3.setState(val);
                }
                else if (childName.equals("/3"))
                {
                    mRelay4.setState(val);
                }
            }
            FLAG_FIREBASE_STREAM_TRIGGER = true;
        }
    }

    Serial.println();
}

//Global function that handles stream timeout
void streamTimeoutCallback(bool timeout)
{
    if (timeout)
    {
        Serial.println();
        Serial.println("Stream timeout, resume streaming...");
        Serial.println();
    }
}
/*---***********---**********---**********---**********---*/

void startFirebase() {
    mFirebaseControl.begin(FIREBASE_HOST, FIREBASE_AUTH);
    mFirebaseControl.setPath(mFirebaseControl.TYPE_PATH_TO_GET, "control/states"); // create parent getter path
    mFirebaseControl.setPath(mFirebaseControl.TYPE_PATH_TO_SET, "control/states"); // create parent setter path
    mFirebaseControl.initMutilStream(childPath, 4, streamCallback, streamTimeoutCallback);

    mFirebaseParameter.begin(FIREBASE_HOST, FIREBASE_AUTH);
    mFirebaseParameter.setPath(mFirebaseParameter.TYPE_PATH_TO_SET, "sensor");
}

void firebaseSetRelayStatus( MyRelay mRelay, uint8_t relayPathPosition) {
    // Make sure connected Firebase
    if (checkInternet())
    {   
        FLAG_FIREBASE_CONTROL_SETTER = true;
        mFirebaseControl.setBoolValue(childPath[relayPathPosition], mRelay.getState());
    }
}

void firebaseSyncRelayStatus()
{
    // TODO Sync database
    if(checkInternet()) {
        FLAG_FIREBASE_CONTROL_SETTER = true;
        
        mFirebaseControl.setBoolValue(childPath[0], mRelay1.getState());
        mFirebaseControl.setBoolValue(childPath[1], mRelay2.getState());
        mFirebaseControl.setBoolValue(childPath[2], mRelay3.getState());
        mFirebaseControl.setBoolValue(childPath[3], mRelay4.getState());
    }
}

void startAllButtonInterrupt();
void stopAllButtonInterrupt();

hw_timer_t *timer0 = NULL;
/**
 * @param chanel 4 timer (0-3)
 * @param interval repeating time
 * @param onTimer() the function is called when timer trigger
 */ 
void setTimerInterrupt(hw_timer_t *timer, int chanel, float interval, void onTimer()) {
    // set prescaler
    timer = timerBegin(chanel, 80, true);
    // set interrupt funtion
    timerAttachInterrupt(timer, onTimer, true);
    // set time (ms)
    interval = interval/1000 * 1000000;
    timerAlarmWrite(timer, interval, true); // true: ascending counter
    // enable alarm
    timerAlarmEnable(timer);
}

void stopTimerInterrupt(hw_timer_t *timer) {
    timerEnd(timer);
    timer = NULL;
}

// Send relay status response to Webserver (AJAX)
void handleRelayStatus() {
    Serial.println("AJAX /getAllStatus");

    String relay1Status;
    String relay2Status;
    String relay3Status;
    String relay4Status;

    if(mRelay1.getState() == ON) {
        relay1Status = "ON";
    }
    else {
        relay1Status = "OFF";
    }

    if(mRelay2.getState() == ON) {
        relay2Status = "ON";
    }
    else {
        relay2Status = "OFF";
    }

    if(mRelay3.getState() == ON) {
        relay3Status = "ON";
    }
    else {
        relay3Status = "OFF";
    }

    if(mRelay4.getState() == ON) {
        relay4Status = "ON";
    }
    else {
        relay4Status = "OFF";
    }

    String statusJson = "{\"RELAY1\": \""+ relay1Status +"\"," +
                "\"RELAY2\": \""+ relay2Status + "\"," +
                "\"RELAY3\": \""+ relay3Status + "\"," +
                "\"RELAY4\": \""+ relay4Status +"\"}";

    MyWebServer.sendJsonResponse(statusJson);
}

/*--- External Interrupt Block ---*/
void  IRAM_ATTR ISR_Button1() {
    if(mButton1.readDigital() == HIGH) {
        Serial.println("Button 1 is pressed");
        mRelay1.toggle();
        mButton1.FLAG_ISPRESSED = true;
        FLAG_OCCUR_BUTTON_INTERRUPT = true;
        stopAllButtonInterrupt();
    }
}

void  IRAM_ATTR ISR_Button2() {
    if(mButton2.readDigital() == HIGH) {
        Serial.println("Button 2 is pressed");
        mRelay2.toggle();
        mButton2.FLAG_ISPRESSED = true;
        FLAG_OCCUR_BUTTON_INTERRUPT = true;
        stopAllButtonInterrupt();
    }
}

void  IRAM_ATTR ISR_Button3() {
    if(mButton3.readDigital() == HIGH) {
        Serial.println("Button 3 is pressed");
        mRelay3.toggle();
        mButton3.FLAG_ISPRESSED = true;
        FLAG_OCCUR_BUTTON_INTERRUPT = true;
        stopAllButtonInterrupt();
    }
}

void  IRAM_ATTR ISR_Button4() {
    if(mButton4.readDigital() == HIGH) {
        Serial.println("Button 4 is pressed");
        mRelay4.toggle();
        mButton4.FLAG_ISPRESSED = true;
        FLAG_OCCUR_BUTTON_INTERRUPT = true;
        stopAllButtonInterrupt();
    }
}

void IRAM_ATTR ISR_Switch() {
    // Accept handle switch trigger
    FLAG_OCCUR_SWITCH_INTERRUPT = true;

    Serial.println(mSwitch.readDigital());
}

void handleButtonTrigger() {
    // Button 1 is pressed
    if(mButton1.FLAG_ISPRESSED) {
        mButton1.FLAG_ISPRESSED = false;

        // firebaseSyncRelayStatus();
        firebaseSetRelayStatus(mRelay1, 0);
    }
    // Button 2 is pressed
    if(mButton2.FLAG_ISPRESSED) {
        mButton2.FLAG_ISPRESSED = false;

        // firebaseSyncRelayStatus();
        firebaseSetRelayStatus(mRelay2, 1);
    }
    // Button 3 is pressed
    if(mButton3.FLAG_ISPRESSED) {
        mButton3.FLAG_ISPRESSED = false;

        // firebaseSyncRelayStatus();
        firebaseSetRelayStatus(mRelay3, 2);
    }
    // Button 4 is pressed
    if(mButton4.FLAG_ISPRESSED) {
        mButton4.FLAG_ISPRESSED = false;

        // firebaseSyncRelayStatus();
        firebaseSetRelayStatus(mRelay4, 3);
    }
}

void handleSwitchTrigger() {
    // Occur switch interrupt
    // if(FLAG_OCCUR_SWITCH_INTERRUPT) {
    //     // No accept handle switch trigger
    //     FLAG_OCCUR_SWITCH_INTERRUPT = false;

    //     // Recycle pressed switch time 
    //     time_ms_previous_pressed_switch = millis();

    //     // Accept change mode
    //     FLAG_CHANGE_MODE = true;
    // }

    // Occur change mode
    if(FLAG_CHANGE_MODE) {
        // Re-check that switch is preesed to avoid noise
        // if(millis() - time_ms_previous_pressed_switch > 50) {
            // No accept change mode
            FLAG_CHANGE_MODE = false;

            // Check switch status
            if(mSwitch.readDigital()) {
                // HIGH: Disiable network mode
                Serial.println("Dsiable Network Mode");
            }
            else {
                // LOW: Enable network mode
                Serial.println("Enable Network Mode");
            }

            restartEsp();
        // } 
    }
}

void startAllButtonInterrupt() {
    Serial.println("Start all button interrupt");

    // HIGH mode is better than RISING
    mButton1.setInterrupt(ISR_Button1, RISING);
    mButton2.setInterrupt(ISR_Button2, RISING);
    mButton3.setInterrupt(ISR_Button3, RISING);
    mButton4.setInterrupt(ISR_Button4, RISING);
}

void stopAllButtonInterrupt() {
    Serial.println("Stop all button interrupt");

    mButton1.stopInterrupt();
    mButton2.stopInterrupt();
    mButton3.stopInterrupt();
    mButton4.stopInterrupt();
}

void onTimer0() {
    if(FLAG_OCCUR_BUTTON_INTERRUPT) {
        FLAG_OCCUR_BUTTON_INTERRUPT = false;

        startAllButtonInterrupt();
    }

    if(FLAG_OCCUR_SWITCH_INTERRUPT) {
        FLAG_CHANGE_MODE = true;
    }
}
//____________________________________________________________________

void readSensorParameter() {
    mPZEM004T.readVoltage();
    mPZEM004T.readCurrent();
    mPZEM004T.readPower();
    mPZEM004T.readEnergy();
}

void firebaseSetSensorParameter() {
    if(checkInternet()) {
        mFirebaseParameter.setFloatValue("/voltage", mPZEM004T.voltage);
        mFirebaseParameter.setFloatValue("/current", mPZEM004T.current);
        mFirebaseParameter.setFloatValue("/power", mPZEM004T.power);
        mFirebaseParameter.setFloatValue("/energy", mPZEM004T.energy);
    }
}

void handleFirebaseSetParameter() {
    if(FLAG_OCCUR_SWITCH_INTERRUPT || FLAG_OCCUR_BUTTON_INTERRUPT ||
        mButton1.FLAG_ISPRESSED || mButton2.FLAG_ISPRESSED || 
        mButton3.FLAG_ISPRESSED || mButton4.FLAG_ISPRESSED) {

        return;
    }

    if(millis() - mPZEM004T.time_ms_previous_read_data > mPZEM004T.TIME_MS_READ_DATA_DELAY) {
        mPZEM004T.time_ms_previous_read_data = millis();

        readSensorParameter();

        firebaseSetSensorParameter();
    }
}

//---------- Handle WebServer ---------------

void handleRelay1ON() {
    Serial.println("AJAX /relay1ON");
    
    mRelay1.on();
    // firebaseSetRelayStatus(mRelay1, 0);
    handleRelayStatus();
}

void handleRelay1OFF() {
    Serial.println("AJAX /relay1OFF");
    
    mRelay1.off();
    // firebaseSetRelayStatus(mRelay1, 0);
    handleRelayStatus();
}

void handleRelay2ON() {
    Serial.println("AJAX /relay2ON");
    
    mRelay2.on();
    // firebaseSetRelayStatus(mRelay2, 0);
    handleRelayStatus();
}

void handleRelay2OFF() {
    Serial.println("AJAX /relay2OFF");
    
    mRelay2.off();
    // firebaseSetRelayStatus(mRelay2, 0);
    handleRelayStatus();
}

void handleRelay3ON() {
    Serial.println("AJAX /relay3ON");
    
    mRelay3.on();
    // firebaseSetRelayStatus(mRelay3, 0);
    handleRelayStatus();
}

void handleRelay3OFF() {
    Serial.println("AJAX /relay3OFF");
    
    mRelay3.off();
    // firebaseSetRelayStatus(mRelay3, 0);
    handleRelayStatus();
}

void handleRelay4ON() {
    Serial.println("AJAX /relay4ON");
    
    mRelay4.on();
    // firebaseSetRelayStatus(mRelay4, 0);
    handleRelayStatus();
}

void handleRelay4OFF() {
    Serial.println("AJAX /relay4OFF");
    
    mRelay4.off();
    // firebaseSetRelayStatus(mRelay4, 0);
    handleRelayStatus();
}

void handleSensorParameter() {
    Serial.println("AJAX /getSensorParameter");

    readSensorParameter();

    String parameterJson = "{\"VOLTAGE\": \""+ String(mPZEM004T.voltage) +"\"," +
            "\"CURRENT\": \""+ String(mPZEM004T.current) + "\"," +
            "\"POWER\": \""+ String(mPZEM004T.power) + "\"," +
            "\"ENERGY\": \""+ String(mPZEM004T.energy) +"\"}";

    MyWebServer.sendJsonResponse(parameterJson);
}

void blinkLed() {
    mLedWiFiConnection.blink();
}

void setup()
{
    Serial.begin(9600);

    WiFi.mode(WIFI_AP_STA);

    // Use for wifi info...
    EEPROM.begin(512);
    delay(50); 

    // Sync time millis
    time_ms_previous_pressed_switch = millis();
    mPZEM004T.time_ms_previous_read_data = millis();

    if(!mSwitch.readDigital()) {
        // Enable network MODE
        mode = 1;

        // Connect to saved wifi
        mSavedWiFi = MyWiFiManager.readWiFiFromEEPROM(10, 50); 
        if(MyWiFiManager.connectToNetwork(mSavedWiFi.getSsid(), mSavedWiFi.getPassword())) {
            // checkInternet();

            startFirebase();

            firebaseSyncRelayStatus();

            MyWiFiManager.internetStatus = true;

            mLedWiFiConnection.on();
        }
        else {
            // Blink LED 1000ms (1s on & 1s off)
            mLedWiFiConnection.setBlinkTimer(1, 1000, blinkLed, 0);

            WiFi.disconnect();
            delay(100);

            mode = 0;
        }
    }
    else {
        // Disiable network MODE
        mode = 0;

        WiFi.disconnect();
        delay(100);
        
        // Blink LED 3000ms (3s on and 3s off)
        mLedWiFiConnection.setBlinkTimer(1, 2000, blinkLed, 0);
    }

    MyWebServer.startSoftAP("NodeMCU32sAcessPoint", "123456789", 4);
    MyWebServer.startWebServer(handleRelay1ON, handleRelay1OFF, handleRelay2ON, handleRelay2OFF,
                                handleRelay3ON, handleRelay3OFF, handleRelay4ON, handleRelay4OFF,
                                handleRelayStatus, handleSensorParameter);

    // Initilize button interrupt
    startAllButtonInterrupt();

    mSwitch.setInterrupt(ISR_Switch, FALLING);

    // Initilize timer interrupt
    setTimerInterrupt(timer0, 0, 2000, onTimer0);

    // Permit stream Firebase
    FLAG_FIREBASE_CONTROL_SETTER = false;
}

void loop()
{
    MyWebServer.handleClient();

    handleSwitchTrigger();
         
    handleButtonTrigger();

    handleFirebaseSetParameter();
}
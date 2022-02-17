#include "Arduino.h"
#include "MyFirebase.h"
#include <FirebaseESP32.h>
#include <WiFi.h>

MyFirebase::MyFirebase()
{
    init();
}

void MyFirebase::init()
{
}

void MyFirebase::begin(const String FIREBASE_HOST, const String FIREBASE_AUTH)
{
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
}

/**
 * Need to implement 2 method in main.cpp
 */
void MyFirebase::initStream(FirebaseData::StreamEventCallback dataAvailableCallback, FirebaseData::StreamTimeoutCallback timeoutCallback)
{
    // set stream
    Firebase.setStreamCallback(firebaseDataGetter, dataAvailableCallback, timeoutCallback);
    // begin stream
    if (!Firebase.beginStream(firebaseDataGetter, pathToGet))
    {
        // Could not begin stream connection, then print out the error detail
        Serial.println(firebaseDataGetter.errorReason());
    }
}

/**
 * Need to implement 2 method in main.cpp
 */
void MyFirebase::initMutilStream(String childPath[],size_t size, FirebaseData::MultiPathStreamEventCallback multiPathDataCallback, FirebaseData::StreamTimeoutCallback timeoutCallback)
{
    //Set the reserved size of stack memory in bytes for internal stream callback processing RTOS task.
    //8192 is the minimum size.
    Firebase.setMultiPathStreamCallback(firebaseDataGetter, multiPathDataCallback, timeoutCallback, 8192);

    if (!Firebase.beginMultiPathStream(firebaseDataGetter, pathToGet, childPath, size))
    {
        Serial.println("------------------------------------");
        Serial.println("Can't begin stream connection...");
        Serial.println("REASON: " + firebaseDataGetter.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }
}

/**
 * Call this method whenever creating or changing path
 *@param path: global path on working with database
 */
void MyFirebase::setPath(const String TYPE_PATH, String path)
{
    if (TYPE_PATH == TYPE_PATH_TO_SET)
    {
        this->pathToSet = path;
    }
    else if (TYPE_PATH == TYPE_PATH_TO_GET)
    {
        this->pathToGet = path;
    }
}

/**
 * Call in dataAvilableCallback function of stream
 * @return String - current global path
 */
String MyFirebase::getPath(const String TYPE_PATH)
{
    if (TYPE_PATH == TYPE_PATH_TO_SET)
    {
        return pathToSet;
    }
    else if (TYPE_PATH == TYPE_PATH_TO_GET)
    {
        return pathToGet;
    }

    return "";
}

/**
 * Set on child path. If need to change parent, call setPath()
 * @param childPath: the child path will be appended after the parent path. This don't make change parent path
 * @param val: the desired value is set
 * @return true - if successful, flase - if failure (can be use for prevent the other stream trigger)
 */
bool MyFirebase::setBoolValue(String childPath, bool val)
{
    if (Firebase.setBool(firebaseDataSeter, pathToSet + childPath, val))
    {
        // Serial.println("PASSED");
        // Serial.println();

        return true;
    }
    else
    {
        // Serial.println("FAILED");
        // Serial.println("REASON: " + firebaseDataSeter.errorReason());
        // Serial.println("------------------------------------");
        // Serial.println();

        return false;
    }
}

/**
 * Set on global (parent) path
 * @param val: the desired value is set
 * @return true - if successful, flase - if failure (can be use for prevent the other stream trigger)
 */
bool MyFirebase::setBoolValue(bool val)
{
    //This will trig the another stream event.
    if (Firebase.setBool(firebaseDataSeter, pathToSet, val))
    {
        // Serial.println("PASSED");
        // Serial.println();

        return true;
    }
    else
    {
        // Serial.println("FAILED");
        // Serial.println("REASON: " + firebaseDataSeter.errorReason());
        // Serial.println("------------------------------------");
        // Serial.println();

        return false;
    }
}

bool MyFirebase::setFloatValue(String childPath, float val)
{
    if (Firebase.setFloat(firebaseDataSeter, pathToSet + childPath, val))
    {
        // Serial.println("PASSED");
        // Serial.println();

        return true;
    }
    else
    {
        // Serial.println("FAILED");
        // Serial.println("REASON: " + firebaseDataSeter.errorReason());
        // Serial.println("------------------------------------");
        // Serial.println();

        return false;
    }
}

bool MyFirebase::setFloatValue(float val)
{
    //This will trig the another stream event.
    if (Firebase.setFloat(firebaseDataSeter, pathToSet, val))
    {
        //   Serial.println("PASSED");
        //   Serial.println();

        return true;
    }
    else
    {
        //   Serial.println("FAILED");
        //   Serial.println("REASON: " + firebaseDataSeter.errorReason());
        //   Serial.println("------------------------------------");
        //   Serial.println();

        return false;
    }
}
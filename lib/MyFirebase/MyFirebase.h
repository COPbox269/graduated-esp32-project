#ifndef MyFirebase_h
#define MyFirebase_h
#endif

#include "Arduino.h"
#include <FirebaseESP32.h>

// Maximum depth of child nodes
// Each path in  data tree must be less than 32 levels deep
#define CHILD_NODES_MAXIMUM 32

class MyFirebase {
  public:
    volatile bool FLAG_FIREBASE_INIT_SUCCESS = false;

    const String TYPE_PATH_TO_SET = "PATH_TO_SET";
    const String TYPE_PATH_TO_GET = "PATH_TO_GET";

    MyFirebase();

    static void begin(const String FIREBASE_HOST, const String FIREBASE_AUTH);

    void setPath(const String TYPE_PATH, String path);

    String getPath(const String TYPE_PATH);

    void initStream(FirebaseData::StreamEventCallback dataAvailableCallback, FirebaseData::StreamTimeoutCallback timeoutCallback);

    void initMutilStream(String childPath[], size_t size, FirebaseData::MultiPathStreamEventCallback dataAvailableCallback, FirebaseData::StreamTimeoutCallback timeoutCallback);
    
    bool setBoolValue(String childPath, bool val);

    bool setBoolValue(bool val);

    bool setFloatValue(String childPath, float val);

    bool setFloatValue(float val);

  private:
    void init();

    // bool isConnected;

    // global (parent) path for firebaseSetter 
    String pathToSet;

    // global (parent) path for firebaseGetter 
    String pathToGet;

    // Define Firebase data object to set data
    FirebaseData firebaseDataSeter;

    // Define Firebase data object to get data (use stream to fetch data)
    FirebaseData firebaseDataGetter;
};
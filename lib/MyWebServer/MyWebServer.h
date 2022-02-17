#ifndef MyWebServer_h
#define MyWebServer_h
#endif

#include <Arduino.h>

class MyWebServerClass
{
public:
    MyWebServerClass();

    MyWebServerClass(String ssid_ap, String password_ap);

    ~MyWebServerClass(); // Detructor no parameter

    void handleClient();

    void startSoftAP(String ssid_ap, String password_ap, int max_connection);

    void startWebServer(void handleRelay1ON(), void handleRelay1OFF(),
                        void handleRelay2ON(), void handleRelay2OFF(),
                        void handleRelay3ON(), void handleRelay3OFF(),
                        void handleRelay4ON(), void handleRelay4OFF(),
                        void handleRelayStatus(), void handleSensorParameter());

    void stop();

    static void handleRoot();

    static void handleWiFiScanner();

    static void handleWiFiConnection();

    static void handleNotFound();

    void sendJsonResponse(String json);

    static void handleClient(unsigned long timeout);

private:
    String ssid_ap;
    String password_ap;
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_MyWebServer)
extern MyWebServerClass MyWebServer;
#endif
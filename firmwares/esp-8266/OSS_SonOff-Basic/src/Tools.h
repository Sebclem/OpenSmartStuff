#ifndef Tools_h
#define Tool_h

#include "Arduino.h"
#include <ArduinoJson.h>

#ifdef ESP8266
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#define SERVER ESP8266WebServer

#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#define SERVER WebServer

#else
#error "Please select ESP8266 Board or ESP32 !"
#endif

#define CONFIG_EEPROM_ADDR 10
#define PASS_EEPROM_ADDR 512

class Tools
{
private:
public:
    Tools();
    static void printWifiStat();
    static void initJsonObject(JsonDocument *connectionObject, String *connectionStr, JsonDocument *stateObject);
    static void saveConfig(DynamicJsonDocument config);
    static bool loadConfig(DynamicJsonDocument *destination);
    static bool checkConfig(DynamicJsonDocument config);
    static DynamicJsonDocument getConfigOject(SERVER *server);
    static void printConfig(DynamicJsonDocument config);
    static String getPassword();
    static void savePassword(String password);
    static String readStringFromEEPROM(int add);
    static void writeStringToEEPROM(int add, String data);
    static char *stringToCharPtr(String s);
    static String ipToString(IPAddress ip);
    static boolean isIp(String str);
};

#endif

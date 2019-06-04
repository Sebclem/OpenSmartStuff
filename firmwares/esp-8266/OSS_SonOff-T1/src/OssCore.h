#ifndef OssCore_h
#define OssCore_h

#include "Arduino.h"
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <DNSServer.h>

#include <EEPROM.h>
#include "Tools.h"
#include <ArduinoOTA.h>

#ifdef ESP8266
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>
#define SERVER ESP8266WebServer
#define SET_HOSTNAME hostname

#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#define SET_HOSTNAME setHostname


#define SERVER WebServer
#else
#error "Please select ESP8266 Board or ESP32 !"
#endif
#include <DNSServer.h>

//-----------------------------------------------------
//-----------------------------------------------------
//                  WEB PAGES
//-----------------------------------------------------
//-----------------------------------------------------
const char header[] = {"<!DOCTYPE html><html><head> <title>SonOff Setting</title><link rel=\"icon\" href=\"data:,\"><link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/></head><body><main id=\"box\">"};

const char footer[] = {"</main></body></html>"};

const char setupPage[] = {"<form method=\"post\"><div class=\"top-blue\"><h3 class=\"white-text\" style=\"text-align: center; font-size: 2rem\"><b>Connect device</b></h3></div><div class=\"divider z-depth-2\"></div><div class=\"divider z-depth-2\" style=\"margin-bottom: 10px\"></div><div class=\"mini-box z-depth-2\"><h4 class=\"white-text\" style=\"margin-top: 16px;margin-bottom: 16px\"><b>Wifi Settings</b></h4></div><div class=\"input-box \"><label for=\"ssid\">Wifi Network Name</label><input id=\"ssid\" class=\"z-depth-2\" name=\"ssid\" type=\"text\" value=\"$ssid\" required/></div><div class=\"input-box\"><label for=\"psk\">Wifi Password</label><input id=\"psk\" class=\"z-depth-2\" name=\"psk\" type=\"text\" value=\"$psk\"/></div><div class=\"mini-box z-depth-2\"><h4 class=\"white-text\" style=\"margin-top: 16px;margin-bottom: 16px\"><b>Server Settings</b></h4></div><div class=\"input-box\"><label for=\"host\">Server host</label><input id=\"host\" class=\"z-depth-2\" name=\"host\" type=\"text\" value=\"$host\" required/></div><div class=\"input-box\"><label for=\"uuid\">Uuid</label><input id=\"uuid\" class=\"z-depth-2\" name=\"uuid\" type=\"text\" value=\"$uuid\" required/></div><div class=\"input-box\"><label for=\"token\">Token</label><input id=\"token\" class=\"z-depth-2\" name=\"token\" type=\"text\" value=\"$token\" required/></div><div style=\"width:100%; text-align: center\"><button type=\"submit\" class=\"z-depth-2\">SAVE</button></div></form>"};

const char homepage[] = {"<div><div class=\"top-blue\"><h3 class=\"white-text\" style=\"text-align: center; font-size: 2rem\"><b>Home</b></h3></div><div class=\"divider z-depth-2\"></div><div class=\"divider z-depth-2\" style=\"margin-bottom: 10px\"></div><div style=\"width:100%; text-align: center;\"> <a type=\"submit\" class=\"z-depth-2 button\" href=\"/config\" style=\"background-color: #00bcd4;margin-bottom: 0\">Settings</a></div><div style=\"width:100%; text-align: center\"> <a type=\"submit\" class=\"z-depth-2 button\" href=\"/setPassword\" style=\"background-color: #00bcd4\">Change Password</a></div></div>"};

const char rebootPage[] = {"<div class=\"top-blue\"><h3 class=\"white-text\" style=\"text-align: center; color: white; font-size: 2rem\"><b>Device Rebooting</b></h3></div><div class=\"divider z-depth-2\"></div><div class=\"divider z-depth-2\" style=\"margin-bottom: 10px\"></div><div class=\"input-box\" style=\"text-align: center; padding-bottom: 5px\"><p>Device is rebooting with you'r new configuration.</p></div>"};

const char passForm[] = {"<form method=\"post\"><div class=\"top-blue\"> <h3 class=\"white-text\" style=\"text-align: center; font-size: 2rem\"><b>Change Password</b></h3> </div><div class=\"divider z-depth-2\"></div><div class=\"divider z-depth-2\" style=\"margin-bottom: 10px\"></div><div class=\"input-box\"><p style=\"color:red;text-align:center\">%message</p></div>%curentPass <div class=\"input-box \"> <label for=\"newPassword\">New Password</label> <input id=\"newPassword\" class=\"z-depth-2\" name=\"newPassword\" type=\"password\" autocomplete=\"new-password\" required/> </div><div style=\"width:100%; text-align: center\"><button type=\"submit\" class=\"z-depth-2\">SAVE</button></div></form>"};

const String currentPassInput = "<div class=\"input-box\"><label for=\"current-pass\">Current Password</label> <input id=\"ssid\" class=\"z-depth-2\" name=\"current-pass\" type=\"password\" autocomplete=\"current-password\" required/></div>";

const char css[] = {"html{font-family:-apple-system,BlinkMacSystemFont,\"Segoe UI\",Roboto,Oxygen-Sans,Ubuntu,Cantarell,\"Helvetica Neue\",sans-serif}body{background-color:#212121;color:white}.top-blue{border-radius:10px 10px 0 0;background-color:#1976D2;overflow:auto}label{font-size:1rem}input{width:100%;background-color:#212121;padding:10px;border-style:none;color:#bdbdbd;border-radius:10px;font-size:1.2rem;margin-top:5px}label{font-size:1.2rem}#box{box-shadow:0 24px 38px 3px rgba(0, 0, 0, 0.14), 0 9px 46px 8px rgba(0, 0, 0, 0.12), 0 11px 15px -7px rgba(0, 0, 0, 0.2);border-radius:10px;background-color:#37474f}@media screen and (max-width: 920px){#box{margin-top:2vh;margin-left:5%;margin-right:5%}.input-box{margin-left:4%;margin-right:4%}}@media screen and (min-width: 920px){#box{margin-top:5vh;margin-left:25%;margin-right:25%}.input-box{margin-left:20%;margin-right:20%}}.divider{height:1px;overflow:hidden;background-color:#bdbdbd}.z-depth-2{box-shadow:0 4px 5px 0 rgba(0, 0, 0, 0.14), 0 1px 10px 0 rgba(0, 0, 0, 0.12), 0 2px 4px -1px rgba(0,0,0,0.3)}.mini-box{background-color:#546e7a;border-radius:10px 10px 10px 10px;padding:0.5%;margin:5px 4% 5px 4%;text-align:center;font-size:1.5rem}.input-box{margin-bottom:10px}button,a.button{background-color:#4caf50;font-size:1rem;padding:10px 30px 10px 30px;border-radius:10px 10px 10px 10px;border-style:none;margin-bottom:20px;margin-top:10px;color:black;text-decoration:none}button:hover{cursor:pointer}a.button{display:inline-block}"};

class OssCore
{

public:
    typedef std::function<void(DynamicJsonDocument *)> CommandCallBackType;
    OssCore(String ApSsid, String hostname, int statusLedPin, CommandCallBackType commandCallBack, CommandCallBackType updateStateCallBack);

    SERVER *server;
    StaticJsonDocument<1024> stateObject;
    void loop();

private:
    enum STATES
    {
        STATES_INIT,
        STATES_NEED_AP,
        STATES_NEED_SETUP,
        STATES_NEED_REBOOT,
        STATES_WIFI_CONNECTION,
        STATES_OK
    };
    byte DNS_PORT = 53;

    WebSocketsClient webSocket;
    StaticJsonDocument<1024> connectionObject;

    String connectionString;
    unsigned long startWifiTime = -1;
    unsigned long lastBlink = -1;
    unsigned long rebootTrigerTime = -1;
    bool ledState = true;
    DNSServer *dnsServer = new DNSServer();
    STATES STATE = STATES_INIT;
    int statusLedPin;
    String apSsid;
    String hostname;

    void startAP();
    void checkWifi();
    void needReboot();
    void startWebServer();
    void startOTA();
    void otaProgress(unsigned int progress, unsigned int total);
    void handleRoot();
    void handleConfigForm();
    void handlePassForm();
    void redirectToCaptive();
    void redirectToPassForm();
    void handleStyle();
    bool checkForm();
    String getForm();
    void startWebSocket();
    void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);
    CommandCallBackType commandCallBack;
    CommandCallBackType updateStateCallBack;
};

#endif

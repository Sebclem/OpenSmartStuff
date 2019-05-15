#include <Arduino.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>

#define WS_PORT 443
#define WS_PATH "/"

#define LED_PIN 13 // 2 for BUILDIN LED
#define ON_OFF_PIN 12

#define WIFI_TIMEOUT 30000 //30 sec
#define REBOOT_SLEEP 10000
#define CONNECTING_BLINK 700
#define SETUP_BLINK 200


//-----------------------------------------------------
//-----------------------------------------------------
//                  WEB PAGES
//-----------------------------------------------------
//-----------------------------------------------------

const char header[] PROGMEM = {"<!DOCTYPE html><html><head> <title>SonOff Setting</title><link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\"></head><body><main id=\"box\" style=\"margin-top:10vh\">"};

const char footer[] PROGMEM = {"</main></body></html>"};

const char setupPage[] PROGMEM = {"<form method=\"post\"><div class=\"top-blue\"><h3 class=\"white-text\" style=\"text-align: center; font-size: 2rem\"><b>Connect device</b></h3></div><div class=\"divider z-depth-2\"></div><div class=\"divider z-depth-2\" style=\"margin-bottom: 10px\"></div><div class=\"mini-box z-depth-2\"><h4 class=\"white-text\" style=\"margin-top: 16px;margin-bottom: 16px\"><b>Wifi Settings</b></h4></div><div class=\"input-box \"><label for=\"ssid\">Wifi Network Name</label><input id=\"ssid\" class=\"z-depth-2\" name=\"ssid\" type=\"text\" value=\"$ssid\" required/></div><div class=\"input-box\"><label for=\"psk\">Wifi Password</label><input id=\"psk\" class=\"z-depth-2\" name=\"psk\" type=\"text\" value=\"$psk\"/></div><div class=\"mini-box z-depth-2\"><h4 class=\"white-text\" style=\"margin-top: 16px;margin-bottom: 16px\"><b>Server Settings</b></h4></div><div class=\"input-box\"><label for=\"host\">Server host</label><input id=\"host\" class=\"z-depth-2\" name=\"host\" type=\"text\" value=\"$host\" required/></div><div class=\"input-box\"><label for=\"uuid\">Uuid</label><input id=\"uuid\" class=\"z-depth-2\" name=\"uuid\" type=\"text\" value=\"$uuid\" required/></div><div class=\"input-box\"><label for=\"token\">Token</label><input id=\"token\" class=\"z-depth-2\" name=\"token\" type=\"text\" value=\"$token\" required/></div><div style=\"width:100%; text-align: center\"><button type=\"submit\" class=\"z-depth-2\">SAVE</button></div></form>"};

const char rebootPage[] PROGMEM = {"<div class=\"top-blue\"><h3 class=\"white-text\" style=\"text-align: center; color: white; font-size: 2rem\"><b>Device Rebooting</b></h3></div><div class=\"divider z-depth-2\"></div><div class=\"divider z-depth-2\" style=\"margin-bottom: 10px\"></div><div class=\"input-box\" style=\"text-align: center; padding-bottom: 5px\"><p>Device is rebooting with you'r new configuration.</p></div>"};

const char passForm[] PROGMEM = {"<form method=\"post\"> <div class=\"top-blue\"> <h3 class=\"white-text\" style=\"text-align: center; font-size: 2rem\"><b>Change Password</b></h3> </div><div class=\"divider z-depth-2\"></div><div class=\"divider z-depth-2\" style=\"margin-bottom: 10px\"></div>%curentPass <div class=\"input-box \"> <label for=\"newPassword\">New Password</label> <input id=\"newPassword\" class=\"z-depth-2\" name=\"newPassword\" type=\"password\" autocomplete=\"new-password\" required/> </div><div style=\"width:100%; text-align: center\"><button type=\"submit\" class=\"z-depth-2\">SAVE</button></div></form>"};

const String currentPassInput = "<div class=\"input-box \"> <label for=\"ssid\">Current Password</label> <input id=\"ssid\" class=\"z-depth-2\" name=\"ssid\" type=\"password\" autocomplete=\"current-password\" required/> </div>";

const char css[] PROGMEM = {"html{font-family:-apple-system,BlinkMacSystemFont,\"Segoe UI\",Roboto,Oxygen-Sans,Ubuntu,Cantarell,\"Helvetica Neue\",sans-serif}body{background-color:#212121;color:white}.top-blue{border-radius:10px 10px 0 0;background-color:#1976D2;overflow:auto}label{font-size:1rem}input{width:100%;background-color:#212121;padding:10px;border-style:none;color:#bdbdbd;border-radius:10px;font-size:1.2rem;margin-top:5px}label{font-size:1.2rem}#box{box-shadow:0 24px 38px 3px rgba(0, 0, 0, 0.14), 0 9px 46px 8px rgba(0, 0, 0, 0.12), 0 11px 15px -7px rgba(0, 0, 0, 0.2);border-radius:10px;background-color:#37474f}@media screen and (max-width: 920px){#box{margin-left:5%;margin-right:5%}}@media screen and (min-width: 920px){#box{margin-left:25%;margin-right:25%}}.divider{height:1px;overflow:hidden;background-color:#bdbdbd}.z-depth-2{box-shadow:0 4px 5px 0 rgba(0, 0, 0, 0.14), 0 1px 10px 0 rgba(0, 0, 0, 0.12), 0 2px 4px -1px rgba(0,0,0,0.3)}.mini-box{background-color:#546e7a;border-radius:10px 10px 10px 10px;padding:0.5%;margin:5px 4% 5px 4%;text-align:center;font-size:1.5rem}.input-box{margin-left:20%;margin-right:20%;margin-bottom:10px}button{background-color:#4caf50;font-size:1rem;padding:10px 30px 10px 30px;border-radius:10px 10px 10px 10px;border-style:none;margin-bottom:20px;margin-top:10px}button:hover{cursor:pointer}"};

//-----------------------------------------------------
//-----------------------------------------------------
//                  GOBAL VAR
//-----------------------------------------------------
//-----------------------------------------------------

const byte DNS_PORT = 53;
ESP8266WebServer server(80);
WebSocketsClient webSocket;
StaticJsonDocument<1024> connectionObject;
StaticJsonDocument<1024> stateObject;
String connectionString;
unsigned long startWifiTime = -1;
unsigned long lastBlink = -1;
unsigned long rebootTrigerTime = -1;
bool ledState = true;
DNSServer *dnsServer = new DNSServer();

const int CONFIG_EEPROM_ADDR  = 10;
const int  PASS_EEPROM_ADDR = 512;

String STATE = "INIT";

//-----------------------------------------------------
//-----------------------------------------------------
//                  FUNCTION
//-----------------------------------------------------
//-----------------------------------------------------
void changeState(bool newState);
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);
void startWebServer();
void handleRoot();
void handleStyle();
bool checkForm();
void handlePassForm();
void redirectToPassForm();
boolean isIp(String str);
void redirectToCaptive();
void printWifiStat();
void initJsonObject();
bool checkConfig(DynamicJsonDocument config);
DynamicJsonDocument getConfigOject();
void printConfig(DynamicJsonDocument config);
void saveConfig(DynamicJsonDocument config);
bool loadConfig(DynamicJsonDocument *destination);
void startAP();
void checkWifi();
void startWebSocket();
void needReboot();
String getForm();
void writeStringToEEPROM(int add, String data);
String readStringFromEEPROM(int add);
String getPassword();
void savePassword(String password);
char * stringToCharPtr(String s);
//-----------------------------------------------------
//-----------------------------------------------------
//                  SETUP
//-----------------------------------------------------
//-----------------------------------------------------
void setup()
{
    Serial.begin(9600);
    Serial.setDebugOutput(false);
    delay(1000);

    Serial.println("Init ESP...");
    Serial.print("Free space:");
    Serial.println(ESP.getFreeSketchSpace());
    ESP.eraseConfig();

    EEPROM.begin(2048);
    pinMode(ON_OFF_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    DynamicJsonDocument config(512);
    bool loadConfError = loadConfig(&config);
    if (loadConfError)
    {
        Serial.println("Find Conf load it.");
        Serial.println("Connecting to " + config["ssid"].as<String>() + " with PSK " + config["psk"].as<String>());
        WiFi.softAPdisconnect();
        WiFi.disconnect();
        WiFi.mode(WIFI_STA);
        delay(100);
        WiFi.begin(config["ssid"].as<char *>(), config["psk"].as<char *>());
        WiFi.setAutoReconnect(true);
        STATE = "WIFI_SETUP";
        startWifiTime = millis();
    }
    else
    {
        Serial.println("Conf not found !");
        STATE = "NEED_AP";
    }
}

//-----------------------------------------------------
//-----------------------------------------------------
//                 LOOP
//-----------------------------------------------------
//-----------------------------------------------------

void loop()
{
    if (STATE == "NEED_AP")
    {
        startAP();
    }
    else if (STATE == "NEED_SETUP")
    {

        dnsServer->processNextRequest();
        server.handleClient();

        if ((millis() - lastBlink) >= SETUP_BLINK)
        {
            ledState = !ledState;
            digitalWrite(LED_PIN, (ledState ? HIGH : LOW));
            lastBlink = millis();
        }
    }
    else if (STATE == "WIFI_SETUP")
    {
        checkWifi();
    }
    else if (STATE == "OK")
    {
        server.handleClient();
        webSocket.loop();
    }
    else if (STATE == "NEED_REBOOT")
    {
        needReboot();
    }
}

//-----------------------------------------------------
//-----------------------------------------------------
//                   STATE FUNCTIONS
//-----------------------------------------------------
//-----------------------------------------------------
String ipToString(IPAddress ip)
{
    String res = "";

    for (int i = 0; i < 3; i++)
    {
        res += String((ip >> (8 * i)) & 0xFF) + ".";
    }

    res += String(((ip >> 8 * 3)) & 0xFF);
    return res;
}

void startAP()
{
    Serial.println("Launching AP...");
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    delay(100);

    WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
    if (WiFi.softAP("OSS_SonOff_Basic", nullptr, 1))
    {
        delay(500); // Without delay I've seen the IP address blank
        Serial.println("AP launch.");
        STATE = "NEED_SETUP";
        dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
        dnsServer->start(DNS_PORT, "*", WiFi.softAPIP());
        startWebServer();
    }
    else
    {
        Serial.println("AP Fail.");
    }
}

void checkWifi()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        if ((millis() - lastBlink) >= CONNECTING_BLINK)
        {
            ledState = !ledState;
            digitalWrite(LED_PIN, (ledState ? HIGH : LOW));
            lastBlink = millis();
        }
        if ((millis() - startWifiTime) >= WIFI_TIMEOUT)
        {
            Serial.println("Wifi connection time out, switching to AP");
            STATE = "NEED_AP";
        }
    }
    else
    {
        Serial.println("WiFi Connected !");
        printWifiStat();
        startWebSocket();
        digitalWrite(LED_PIN, HIGH);
        initJsonObject();
        startWebServer();
        STATE = "OK";
    }
}

void needReboot()
{
    server.handleClient();
    webSocket.disconnect();
    if ((millis() - rebootTrigerTime) >= REBOOT_SLEEP)
    {
        ESP.restart();
    }
}

//-----------------------------------------------------
//-----------------------------------------------------
//                  WEB SERVER
//-----------------------------------------------------
//-----------------------------------------------------

void startWebServer()
{
    Serial.println("Starting Web Server...");
    server.on("/", handleRoot);
    server.on("/style.css", handleStyle);
    server.on("/setPassword", handlePassForm);
    server.onNotFound([]() {
        redirectToCaptive();
    });
    server.begin();
}

void handleRoot()
{

    if (!isIp(server.hostHeader()))
    {
        redirectToCaptive();
        return;
    }
    String pass = getPassword();
    Serial.println("Pass: " + pass);
    if (pass.length() == 0)
    {
        redirectToPassForm();
    }
    char * passArray = stringToCharPtr(pass);
    if (!server.authenticate("admin", passArray))
    {
        return server.requestAuthentication();
    }

    if (server.hasArg("ssid"))
    {
        Serial.println("[Web] From detected");
        if (checkForm())
        {
            Serial.println("Form valid !");
            DynamicJsonDocument jsonConf = getConfigOject();
            saveConfig(jsonConf);
            DynamicJsonDocument loaded(1024);
            if (loadConfig(&loaded))
            {
                Serial.println("Conf loaded:");
                printConfig(jsonConf);
                server.send(200, "text/html", String(header) + rebootPage + footer);
                STATE = "NEED_REBOOT";
                rebootTrigerTime = millis();
            }
            else
            {
                Serial.println("Conf load fail");
                server.send(200, "text/html", getForm());
            }
        }
        else
        {
            Serial.println("Form invalid !");
            server.send(200, "text/html", getForm());
        }
    }
    else
        server.send(200, "text/html", getForm());
}

void handlePassForm()
{
    String pass = getPassword();
    Serial.println("Pass: " + pass);
    if (pass.length() == 0)
    {
        if (server.hasArg("newPassword"))
        {
            savePassword(server.arg("newPassword"));
            redirectToCaptive();
        }
        else
        {
            String form = String(passForm);
            form.replace("%curentPass", "");
            server.send(200, "text/html", String(header) + form);
        }
    }
    else
    {
        char * passArray = stringToCharPtr(pass);
        if (!server.authenticate("admin", passArray))
        {
            return server.requestAuthentication();
        }
        else{
            String form = String(passForm);
            form.replace("%curentPass", currentPassInput);
            server.send(200, "text/html", String(header) + form);
        }
    }
}

void redirectToCaptive()
{
    Serial.println("Request redirected to captive portal: " + server.hostHeader());
    server.sendHeader("Location", "http://192.168.4.1", true); //Redirect to our html web page
    server.send(302, "text/plane", "");
}

void redirectToPassForm()
{
    Serial.println("No password, redirect to pass form.");
    server.sendHeader("Location", "/setPassword", true); //Redirect to our html web page
    server.send(307, "text/plane", "");
}
void handleStyle()
{
    server.send(200, "text/css", css);
}

bool checkForm()
{
    return server.hasArg("ssid") && server.hasArg("psk") && server.hasArg("host") && server.hasArg("uuid") && server.hasArg("token");
}

String getForm()
{
    String temp = String(setupPage);
    Serial.println("ok");
    DynamicJsonDocument config(1024);
    bool loadConfError = loadConfig(&config);
    if (!loadConfError)
    {
        temp.replace("$ssid", "");
        temp.replace("$psk", "");
        temp.replace("$host", "");
        temp.replace("$uuid", "");
        temp.replace("$token", "");
    }
    else
    {
        temp.replace("$ssid", config["ssid"].as<String>());
        temp.replace("$psk", config["psk"].as<String>());
        temp.replace("$host", config["host"].as<String>());
        temp.replace("$uuid", config["uuid"].as<String>());
        temp.replace("$token", config["token"].as<String>());
    }
    temp = header + temp + footer;
    return temp;
}

boolean isIp(String str)
{
    for (size_t i = 0; i < str.length(); i++)
    {
        int c = str.charAt(i);
        if (c != '.' && (c < '0' || c > '9'))
        {
            return false;
        }
    }
    return true;
}

//-----------------------------------------------------
//-----------------------------------------------------
//                  WEB SOCKET AND LOGIC
//-----------------------------------------------------
//-----------------------------------------------------

void startWebSocket()
{

    DynamicJsonDocument config(1024);
    loadConfig(&config);
    Serial.println("Connecting to Websocket...");
    webSocket.beginSSL(config["host"].as<String>(), WS_PORT, WS_PATH);
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000);

    // start heartbeat (optional)
    // ping server every 15000 ms
    // expect pong from server within 3000 ms
    // consider connection disconnected if pong is not received 2 times
    webSocket.enableHeartbeat(3000, 2000, 2);
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{

    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[WSc] Disconnected!\n");
        delay(10000);
        break;
    case WStype_CONNECTED:

        Serial.printf("[WSc] Connected to url: %s\n", payload);
        Serial.println("[WSc] Send AUTH message");
        webSocket.sendTXT(connectionString);

        break;
    case WStype_TEXT:
        Serial.printf("[WSc] get text: %s\n", payload);

        DynamicJsonDocument received(200);

        DeserializationError error = deserializeJson(received, payload);
        if (error)
        {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.c_str());
        }
        else
        {
            if (received["type"] == "AUTH")
            {
                if (received["state"] == "SUCCESS")
                {
                    Serial.println("Authentication success !");
                }
                else
                {
                    Serial.println("Authentication fail, please check you'r token and uuid !");
                }
            }
            else if (received["type"] == "GET_STATE")
            {
                String str;
                serializeJson(stateObject, str);
                webSocket.sendTXT(str);
            }
            else if (received["type"] == "COMMAND")
            {
                bool command = received["on"];
                Serial.printf("Receive commande on: %s\n", (command ? "true" : "false"));
                changeState(command);
                String str;
                serializeJson(stateObject, str);
                webSocket.sendTXT(str);
            }
        }
        break;
    }
}

void changeState(bool newState)
{
    digitalWrite(ON_OFF_PIN, (newState ? HIGH : LOW));
    digitalWrite(LED_PIN, (!newState ? HIGH : LOW));
    stateObject["state"]["on"] = newState;
}

//-----------------------------------------------------
//-----------------------------------------------------
//                  TOOLS
//-----------------------------------------------------
//-----------------------------------------------------

void printWifiStat()
{
    Serial.print("Wifi address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Mac address: ");
    Serial.println(WiFi.macAddress());
    Serial.print("RSSI:");
    Serial.println(WiFi.RSSI());
}

void initJsonObject()
{
    DynamicJsonDocument config(1024);
    loadConfig(&config);
    connectionObject["type"] = "AUTH";
    connectionObject["uuid"] = config["uuid"];
    connectionObject["token"] = config["token"];
    serializeJson(connectionObject, connectionString);

    stateObject["type"] = "STATE";
    stateObject["uuid"] = config["uuid"];
    JsonObject state = stateObject.createNestedObject("state");
    state["on"] = false;
}

void saveConfig(DynamicJsonDocument config)
{
    String data;
    serializeJson(config, data);
    writeStringToEEPROM(CONFIG_EEPROM_ADDR, data);
}

bool loadConfig(DynamicJsonDocument *destination)
{
    String str = readStringFromEEPROM(CONFIG_EEPROM_ADDR);
    DeserializationError error = deserializeJson(*destination, str);
    if (error)
    {
        return false;
    }
    return checkConfig(*destination);
}

bool checkConfig(DynamicJsonDocument config)
{
    return config.containsKey("ssid") && config.containsKey("psk") && config.containsKey("host") && config.containsKey("uuid") && config.containsKey("token");
}

DynamicJsonDocument getConfigOject()
{
    DynamicJsonDocument config(1024);
    config["ssid"] = server.arg("ssid");
    config["psk"] = server.arg("psk");
    config["host"] = server.arg("host");
    config["uuid"] = server.arg("uuid");
    config["token"] = server.arg("token");

    return config;
}

void printConfig(DynamicJsonDocument config)
{

    Serial.print("SSID = ");
    Serial.println(config["ssid"].as<String>());
    Serial.print("PSK = ");
    Serial.println(config["psk"].as<String>());
    Serial.print("Host = ");
    Serial.println(config["host"].as<String>());
    Serial.print("UUID = ");
    Serial.println(config["uuid"].as<String>());
    Serial.print("TOKEN = ");
    Serial.println(config["token"].as<String>());
}

String getPassword()
{

    String read = readStringFromEEPROM(PASS_EEPROM_ADDR);
    if (read.startsWith("p:"))
    {
        return read.substring(2);
    }
    else
    {
        return String("");
    }
}

void savePassword(String password)
{
    writeStringToEEPROM(PASS_EEPROM_ADDR, "p:" + password);
}

String readStringFromEEPROM(int add)
{
    char data[2048];
    int len = 0;
    unsigned char k;
    k = EEPROM.read(add);
    
    while (k != '\0' && len < 500) //Read until null character
    {
        k = EEPROM.read(add + len);
        data[len] = k;
        len++;
    }
    data[len] = '\0';

    return String(data);
}

void writeStringToEEPROM(int add, String data)
{
    int _size = data.length();
    int i;
    for (i = 0; i < _size; i++)
    {
        EEPROM.write(add + i, data[i]);
    }
    EEPROM.write(add + _size, '\0'); //Add termination null character for String Data
    EEPROM.commit();
}


char * stringToCharPtr(String s){
    unsigned int bufSize = s.length() + 1; //String length + null terminator
	char* ret = new char[bufSize];
	s.toCharArray(ret, bufSize);
	return ret;
}

#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>


// #define WS_HOST "10.3.0.225"
// #define WS_PORT 3000
// #define WS_PATH "/"


#define WS_HOST "smart-stuff.seb6596.ovh"
#define WS_PORT 443
#define WS_PATH "/"

// #define SSID "IMERIR"
// #define WIFI_PSK ""


#define SSID "pico_seb"
#define WIFI_PSK "box_sebastien"

#define UUID "123456789"
#define TOKEN "987654321"

#define ON_OFF_PIN LED_BUILTIN






WebSocketsClient webSocket;
StaticJsonDocument<300> connectionObject;
StaticJsonDocument<300> stateObject;
String connectionString;

int LED_BUILTIN = 22;



void changeState(bool newState);



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
        if (error){
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.c_str());
        }
        else{
            if(received["type"] == "AUTH"){
                if(received["state"] == "SUCCESS"){
                    Serial.println("Authentication success !");
                }else{
                    Serial.println("Authentication fail, please check you'r token and uuid !");
                }
            }else if(received["type"] == "GET_STATE"){
                String str;
                serializeJson(stateObject, str);
                webSocket.sendTXT(str);
            }else if(received["type"] == "COMMAND"){
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

void setup()
{
    Serial.begin(9600);
    Serial.setDebugOutput(true);

    // Prebuild json object
    connectionObject["type"] = "AUTH";
    connectionObject["uuid"] = UUID;
    connectionObject["token"] = TOKEN;

    stateObject["type"] = "STATE";
    stateObject["uuid"] = UUID;
    JsonObject state = stateObject.createNestedObject("state");
    state["on"] = false;



    serializeJson(connectionObject, connectionString);

    pinMode(LED_BUILTIN, OUTPUT);

    Serial.println("Init ESP...");

    WiFi.begin(SSID, WIFI_PSK);
    WiFi.setAutoReconnect(true);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("Connected to WiFi network");
    Serial.print("Wifi address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Mac address: ");
    Serial.println(WiFi.macAddress());
    Serial.print("RSSI:");
    Serial.println(WiFi.RSSI());

    Serial.println("Connecting to Websocket...");
    webSocket.beginSSL(WS_HOST, WS_PORT, WS_PATH);
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000);

    // start heartbeat (optional)
    // ping server every 15000 ms
    // expect pong from server within 3000 ms
    // consider connection disconnected if pong is not received 2 times
    webSocket.enableHeartbeat(3000, 2000, 2);
}
void loop()
{
    webSocket.loop();
}

void changeState(bool newState){
    digitalWrite(ON_OFF_PIN, (newState ? HIGH : LOW));
    stateObject["state"]["on"] = newState;
}
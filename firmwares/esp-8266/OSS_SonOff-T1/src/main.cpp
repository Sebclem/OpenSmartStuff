#include <Arduino.h>
#include <ArduinoJson.h>
#include "OssCore.h"
#include <Wire.h>
#include <SPI.h>

// #define T1_1GANG
#define T1_2GANG
// #define T1_3GANG

#define LED_PIN 13 // 13 for BUILDIN LED

#ifdef T1_3GANG
#define BTN01_PIN 0
#define RELAY_01 12
#define BTN03_PIN 9
#define RELAY_03 4
#endif

#ifdef T1_2GANG
#define BTN01_PIN 0
#define RELAY_01 12
#define BTN03_PIN 9
#define RELAY_03 5
#endif

#if defined(T1_3GANG) || defined(T1_1GANG)
#define BTN02_PIN 10
#define RELAY_02 5
#endif

OssCore *ossCore;
bool relay01State = false;
bool relay02State = false;
bool relay03State = false;

void changeState(DynamicJsonDocument *received);
void updateState(DynamicJsonDocument *received);

#if defined(T1_3GANG) || defined(T1_2GANG)
void ICACHE_RAM_ATTR btn01Interupt();
void ICACHE_RAM_ATTR btn03Interupt();
#endif

#if defined(T1_3GANG) || defined(T1_1GANG)
void ICACHE_RAM_ATTR btn02Interupt();
#endif


void setup()
{
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    interrupts();

    ossCore = new OssCore("OSS_SonOff_T1", "OSS_SonOff_T1", LED_PIN, changeState, updateState);

    ossCore->stateObject.createNestedObject("state");

#if defined(T1_3GANG) || defined(T1_2GANG)
    pinMode(RELAY_01, OUTPUT);
    pinMode(RELAY_03, OUTPUT);
    digitalWrite(RELAY_01, LOW);
    digitalWrite(RELAY_03, LOW);
    pinMode(BTN01_PIN, INPUT);
    pinMode(BTN03_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(BTN01_PIN), btn01Interupt, FALLING);
    attachInterrupt(digitalPinToInterrupt(BTN03_PIN), btn03Interupt, FALLING);
#endif

#if defined(T1_3GANG) || defined(T1_1GANG)
    pinMode(RELAY_02, OUTPUT);
    digitalWrite(RELAY_02, LOW);
    pinMode(BTN02_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(BTN02_PIN), btn02Interupt, FALLING);
#endif
}

void loop()
{
    ossCore->loop();
    
}

void changeState(DynamicJsonDocument *received)
{
    bool command = (*received)["on"];
    switch ((*received)["option"]["id"].as<int>())
    {
    case 1:
#ifdef T1_1GANG
        relay02State = command;
        digitalWrite(RELAY_02, (relay02State ? HIGH : LOW));
#endif
#if defined(T1_3GANG) || defined(T1_2GANG)
        relay01State = command;
        digitalWrite(RELAY_01, (relay01State ? HIGH : LOW));
#endif
        break;

    case 2:
#ifdef T1_2GANG
        relay03State = command;
        digitalWrite(RELAY_03, (relay03State ? HIGH : LOW));
#endif
#ifdef T1_3GANG
        relay02State = command;
        digitalWrite(RELAY_02, (relay02State ? HIGH : LOW));
#endif
        break;

    case 3:
#ifdef T1_3GANG
        relay03State = command;
        digitalWrite(RELAY_03, (relay03State ? HIGH : LOW));
#endif
        break;

    default:
        break;
    }
}

void updateState(DynamicJsonDocument *received)
{
    
    ossCore->stateObject["state"].remove("on");


    switch ((*received)["option"]["id"].as<int>())
    {
    case 1:
#ifdef T1_1GANG
        ossCore->stateObject["state"]["on"] = relay02State;
#endif
#if defined(T1_3GANG) || defined(T1_2GANG)
        ossCore->stateObject["state"]["on"] = relay01State;
#endif
        break;

    case 2:
#ifdef T1_2GANG
        ossCore->stateObject["state"]["on"] = relay03State;
#endif
#ifdef T1_3GANG
        ossCore->stateObject["state"]["on"] = relay02State;
#endif
        break;

    case 3:
#ifdef T1_3GANG
        ossCore->stateObject["state"]["on"] = relay03State;
#endif
        break;

    default:
        break;
    }
}

#if defined(T1_3GANG) || defined(T1_2GANG)
void ICACHE_RAM_ATTR btn01Interupt()
{
    relay01State = !relay01State;
    digitalWrite(RELAY_01, (relay01State ? HIGH : LOW));
}

void ICACHE_RAM_ATTR btn03Interupt()
{
    relay03State = !relay03State;
    digitalWrite(RELAY_03, (relay03State ? HIGH : LOW));
}
#endif

#if defined(T1_3GANG) || defined(T1_1GANG)
void ICACHE_RAM_ATTR btn02Interupt()
{
    relay02State = !relay02State;
    digitalWrite(RELAY_02, (relay02State ? HIGH : LOW));
}
#endif
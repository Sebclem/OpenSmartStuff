#include <Arduino.h>
#include <ArduinoJson.h>
#include "OssCore.h"

#define LED_PIN 2 // 13 for BUILDIN LED
#define ON_OFF_PIN 12

OssCore *ossCore;

bool isOn = false;

void changeState(DynamicJsonDocument *received);
void updateState();

void setup()
{
    ossCore = new OssCore("OSS_SonOff", "test", LED_PIN, changeState, updateState);
    JsonObject state = ossCore->stateObject.createNestedObject("state");
    state["on"] = false;
    pinMode(ON_OFF_PIN, OUTPUT);
}

void loop()
{

    ossCore->loop();
}

void changeState(DynamicJsonDocument *received)
{
    isOn = (*received)["on"];
    Serial.println(isOn);
    digitalWrite(ON_OFF_PIN, (isOn ? HIGH : LOW));
    digitalWrite(LED_PIN, (!isOn ? HIGH : LOW));
}

void updateState()
{
    ossCore->stateObject["state"]["on"] = isOn;
}
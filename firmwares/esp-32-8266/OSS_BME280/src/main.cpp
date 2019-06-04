#include <Arduino.h>
#include <ArduinoJson.h>
#include "OssCore.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define LED_PIN 2 // 13 for BUILDIN LED

OssCore *ossCore;
Adafruit_BME280 bme;

void changeState(DynamicJsonDocument *received);
void updateState(DynamicJsonDocument *received);

void setup()
{
    ossCore = new OssCore("OSS_BME280", "OSS_BME280", LED_PIN, changeState, updateState);
    
    JsonObject state = ossCore->stateObject.createNestedObject("state");
    state["temperature"] = 0.0;
    state["humidity"] = 0.0;
    state["presure"] = 0.0;
    if (!bme.begin(0x76)) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    bme.setSampling(Adafruit_BME280::MODE_NORMAL, Adafruit_BME280::SAMPLING_X16,Adafruit_BME280::SAMPLING_X16, Adafruit_BME280::SAMPLING_X16, Adafruit_BME280::FILTER_OFF, Adafruit_BME280::STANDBY_MS_0_5);

    
}

void loop()
{

    ossCore->loop();
}

void changeState(DynamicJsonDocument *received)
{

}

void updateState(DynamicJsonDocument *received)
{
    bme.takeForcedMeasurement();
    ossCore->stateObject["state"]["temperature"] = bme.readTemperature()-1;
    ossCore->stateObject["state"]["humidity"] = bme.readHumidity();
    ossCore->stateObject["state"]["presure"] = bme.readPressure();
}
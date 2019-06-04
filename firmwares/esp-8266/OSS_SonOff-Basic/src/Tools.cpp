

#include <Arduino.h>
#include <ArduinoJson.h>
#include "Tools.h"
#include <EEPROM.h>


Tools::Tools() {}

void Tools::printWifiStat()
{
    Serial.print("[WIFI] Wifi address: ");
    Serial.println(WiFi.localIP());
    Serial.print("[WIFI] Mac address: ");
    Serial.println(WiFi.macAddress());
    Serial.print("[WIFI] RSSI:");
    Serial.println(WiFi.RSSI());
}

void Tools::initJsonObject(JsonDocument *connectionObject, String *connectionStr, JsonDocument *stateObject)
{
    DynamicJsonDocument config(1024);
    loadConfig(&config);
    (*connectionObject)["type"] = "AUTH";
    (*connectionObject)["uuid"] = config["uuid"];
    (*connectionObject)["token"] = config["token"];
    serializeJson(*connectionObject, *connectionStr);

    (*stateObject)["type"] = "STATE";
    (*stateObject)["uuid"] =  config["uuid"];
}

void Tools::saveConfig(DynamicJsonDocument config)
{
    String data;
    serializeJson(config, data);
    writeStringToEEPROM(CONFIG_EEPROM_ADDR, data);
}

bool Tools::loadConfig(DynamicJsonDocument *destination)
{
    String str = readStringFromEEPROM(CONFIG_EEPROM_ADDR);
    DeserializationError error = deserializeJson(*destination, str);
    if (error)
    {
        return false;
    }
    return checkConfig(*destination);
}

bool Tools::checkConfig(DynamicJsonDocument config)
{
    return config.containsKey("ssid") && config.containsKey("psk") && config.containsKey("host") && config.containsKey("uuid") && config.containsKey("token");
}

DynamicJsonDocument Tools::getConfigOject(SERVER *server)
{
    DynamicJsonDocument config(1024);
    config["ssid"] = server->arg("ssid");
    config["psk"] = server->arg("psk");
    config["host"] = server->arg("host");
    config["uuid"] = server->arg("uuid");
    config["token"] = server->arg("token");

    return config;
}

void Tools::printConfig(DynamicJsonDocument config)
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

String Tools::getPassword()
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

void Tools::savePassword(String password)
{
    writeStringToEEPROM(PASS_EEPROM_ADDR, "p:" + password);
}

String Tools::readStringFromEEPROM(int add)
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

void Tools::writeStringToEEPROM(int add, String data)
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

char *Tools::stringToCharPtr(String s)
{
    unsigned int bufSize = s.length() + 1; //String length + null terminator
    char *ret = new char[bufSize];
    s.toCharArray(ret, bufSize);
    return ret;
}

String Tools::ipToString(IPAddress ip)
{
    String res = "";

    for (int i = 0; i < 3; i++)
    {
        res += String((ip >> (8 * i)) & 0xFF) + ".";
    }

    res += String(((ip >> 8 * 3)) & 0xFF);
    return res;
}

boolean Tools::isIp(String str)
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
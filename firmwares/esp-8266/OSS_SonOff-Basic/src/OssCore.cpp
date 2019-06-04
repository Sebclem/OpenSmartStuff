
#include "OssCore.h"

#define WS_PORT 443
#define WS_PATH "/"

#define WIFI_TIMEOUT 30000 //30 sec
#define REBOOT_SLEEP 10000
#define CONNECTING_BLINK 700
#define SETUP_BLINK 200

//-----------------------------------------------------
//                  SETUP
//-----------------------------------------------------
OssCore::OssCore(String apSsid, String hostname, int statusLedPin, CommandCallBackType commandCallBack, CommandCallBackType updateStateCallBack)
{
    this->updateStateCallBack = updateStateCallBack;
    this->commandCallBack = commandCallBack;
    this->server = new SERVER(80);
    this->apSsid = apSsid;
    this->hostname = hostname;
    this->statusLedPin = statusLedPin;

    Serial.begin(115200);
    Serial.setDebugOutput(true);
    delay(1000);

    Serial.println("\nInit ESP...");
    Serial.print("[INIT] Free space:");
    Serial.println(ESP.getFreeSketchSpace());
    EEPROM.begin(2048);
    pinMode(statusLedPin, OUTPUT);
    digitalWrite(statusLedPin, HIGH);
    WiFi.SET_HOSTNAME(Tools::stringToCharPtr(this->hostname));

    DynamicJsonDocument config(512);
    bool loadConfError = Tools::loadConfig(&config);
    if (loadConfError)
    {
        Serial.println("[INIT] Find Conf load it.");
        Serial.println("[WIFI] Connecting to " + config["ssid"].as<String>() + " with PSK " + config["psk"].as<String>());
        WiFi.softAPdisconnect();
        WiFi.disconnect();
        WiFi.mode(WIFI_STA);
        delay(100);
        WiFi.begin(config["ssid"].as<char *>(), config["psk"].as<char *>());
        WiFi.setAutoReconnect(true);
        this->STATE = STATES_WIFI_CONNECTION;
        this->startWifiTime = millis();
    }
    else
    {
        Serial.println("[INIT] Conf not found !");
        this->STATE = STATES_NEED_AP;
    }
    return;
}

//-----------------------------------------------------
//-----------------------------------------------------
//                 LOOP
//-----------------------------------------------------
//-----------------------------------------------------

void OssCore::loop()
{

    switch (OssCore::STATE)
    {
    case STATES_NEED_AP:
        startAP();
        break;

    case STATES_NEED_SETUP:
        dnsServer->processNextRequest();
        server->handleClient();
        if ((millis() - lastBlink) >= SETUP_BLINK)
        {
            ledState = !ledState;
            digitalWrite(statusLedPin, (ledState ? HIGH : LOW));
            lastBlink = millis();
        }
        break;

    case STATES_NEED_REBOOT:
        needReboot();
        break;

    case STATES_WIFI_CONNECTION:
        checkWifi();
        break;

    case STATES_OK:
        server->handleClient();
        webSocket.loop();
        ArduinoOTA.handle();
        break;
    }
}

//-----------------------------------------------------
//-----------------------------------------------------
//                   STATE FUNCTIONS
//-----------------------------------------------------
//-----------------------------------------------------

void OssCore::startAP()
{
    Serial.println("[WIFI] Launching AP...");
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    delay(100);

    WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
    if (WiFi.softAP(Tools::stringToCharPtr(this->apSsid), "", 4))
    {
        delay(500); // Without delay I've seen the IP address blank
        Serial.println("[WIFI] AP launch.");
        STATE = STATES_NEED_SETUP;
        dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
        dnsServer->start(DNS_PORT, "*", WiFi.softAPIP());
        this->startWebServer();
    }
    else
    {
        Serial.println("AP Fail.");
    }
}

void OssCore::checkWifi()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        if ((millis() - lastBlink) >= CONNECTING_BLINK)
        {
            this->ledState = !this->ledState;
            digitalWrite(statusLedPin, (this->ledState ? HIGH : LOW));
            this->lastBlink = millis();
        }
        if ((millis() - this->startWifiTime) >= WIFI_TIMEOUT)
        {
            Serial.println("[WIFI] Wifi connection time out, switching to AP");
            this->STATE = STATES_NEED_AP;
        }
    }
    else
    {
        Serial.println("[WIFI] WiFi Connected !");
        Tools::printWifiStat();
        this->startWebSocket();
        digitalWrite(statusLedPin, HIGH);
        Tools::initJsonObject(&connectionObject, &connectionString, &stateObject);
        this->startWebServer();
        startOTA();
        STATE = STATES_OK;
    }
}

void OssCore::needReboot()
{
    server->handleClient();
    this->webSocket.disconnect();
    if ((millis() - this->rebootTrigerTime) >= REBOOT_SLEEP)
    {
        ESP.restart();
    }
}

void OssCore::startOTA(){
    ArduinoOTA.setHostname(Tools::stringToCharPtr(this->hostname));

    ArduinoOTA.setPassword(Tools::stringToCharPtr(Tools::getPassword()));

    ArduinoOTA.onStart([]() {
        Serial.println("[OTA] OTA Started...");
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\n[OTA] OTA Finish");
    });
    ArduinoOTA.onProgress(std::bind(&OssCore::otaProgress, this,std::placeholders::_1, std::placeholders::_2));
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("[OTA] Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    ArduinoOTA.begin();
}

void OssCore::otaProgress(unsigned int progress, unsigned int total){
    ledState = !ledState;
    digitalWrite(statusLedPin, (ledState ? HIGH : LOW));
    Serial.printf("[OTA] Progress: %u%%\r", (progress / (total / 100)));
}

//-----------------------------------------------------
//-----------------------------------------------------
//                  WEB SERVER
//-----------------------------------------------------
//-----------------------------------------------------

void OssCore::startWebServer()
{
    Serial.println("[WEB] Starting Web server...");

    // server->setRSACert(new BearSSL::X509List(serverCert), new BearSSL::PrivateKey(serverKey));
    server->on("/", std::bind(&OssCore::handleRoot, this));
    server->on("/style.css", std::bind(&OssCore::handleStyle, this));
    server->on("/setPassword", std::bind(&OssCore::handlePassForm, this));
    server->on("/config", std::bind(&OssCore::handleConfigForm, this));
    server->onNotFound(std::bind(&OssCore::redirectToCaptive, this));
    server->begin();
}

void OssCore::handleRoot()
{

    if (STATE == STATES_NEED_SETUP && !Tools::isIp(server->hostHeader()))
    {
        redirectToCaptive();
        return;
    }

    String pass = Tools::getPassword();
    if (pass.length() == 0)
    {
        redirectToPassForm();
        return;
    }

    char *passArray = Tools::stringToCharPtr(pass);
    if (!server->authenticate("admin", passArray))
    {
        return server->requestAuthentication();
    }

    if (STATE == STATES_OK)
    {
        server->send(200, "text/html", String(header) + homepage + footer);
    }
    else if (STATE == STATES_NEED_SETUP)
    {
        Serial.println("[WEB] Redirect to Conf form.");
        server->sendHeader("Location", "/config", true); //Redirect to our html web page
        server->send(307, "text/plane", "");
    }
}

void OssCore::handlePassForm()
{
    String pass = Tools::getPassword();
    if (pass.length() == 0)
    {
        if (server->hasArg("newPassword"))
        {
            Tools::savePassword(server->arg("newPassword"));
            redirectToCaptive();
        }
        else
        {
            String form = String(passForm);
            form.replace("%curentPass", "");
            form.replace("%message", "");
            server->send(200, "text/html", String(header) + form);
        }
    }
    else
    {
        char *passArray = Tools::stringToCharPtr(pass);
        if (!server->authenticate("admin", passArray))
        {
            return server->requestAuthentication();
        }
        else
        {
            if (server->hasArg("newPassword"))
            {
                if (server->arg("current-pass") == pass)
                {
                    Tools::savePassword(server->arg("newPassword"));
                    redirectToCaptive();
                }
                else
                {
                    String form = String(passForm);
                    form.replace("%curentPass", currentPassInput);
                    form.replace("%message", "Password does not match");
                    server->send(200, "text/html", String(header) + form + footer);
                }
            }
            String form = String(passForm);
            form.replace("%curentPass", currentPassInput);
            form.replace("%message", "");
            server->send(200, "text/html", String(header) + form + footer);
        }
    }
}

void OssCore::handleConfigForm()
{
    if (server->hasArg("ssid"))
    {
        Serial.println("[Web] From detected");
        if (checkForm())
        {
            Serial.println("[WEB] Form valid !");
            DynamicJsonDocument jsonConf = Tools::getConfigOject(server);
            Tools::saveConfig(jsonConf);
            DynamicJsonDocument loaded(1024);
            if (Tools::loadConfig(&loaded))
            {
                Serial.println("[WEB] Conf loaded:");
                Tools::printConfig(jsonConf);
                server->send(200, "text/html", String(header) + rebootPage + footer);
                STATE = STATES_NEED_REBOOT;
                rebootTrigerTime = millis();
            }
            else
            {
                Serial.println("[WEB] Conf load fail");
                server->send(200, "text/html", getForm());
            }
        }
        else
        {
            Serial.println("[WEB] Form invalid !");
            server->send(200, "text/html", getForm());
        }
    }
    else
    {

        server->send(200, "text/html", getForm());
    }
}

void OssCore::handleStyle()
{
    server->send(200, "text/css", css);
}

void OssCore::redirectToCaptive()
{
    Serial.println("[WEB] Request redirected to captive portal: " + server->hostHeader());
    server->sendHeader("Location", "http://192.168.4.1", true); //Redirect to our html web page
    server->send(302, "text/plane", "");
}

void OssCore::redirectToPassForm()
{
    Serial.println("[WEB] No password, redirect to pass form.");
    server->sendHeader("Location", "/setPassword", true); //Redirect to our html web page
    server->send(307, "text/plane", "");
}

bool OssCore::checkForm()
{
    return server->hasArg("ssid") && server->hasArg("psk") && server->hasArg("host") && server->hasArg("uuid") && server->hasArg("token");
}

String OssCore::getForm()
{
    String temp = String(setupPage);
    DynamicJsonDocument config(1024);
    bool loadConfError = Tools::loadConfig(&config);
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

//-----------------------------------------------------
//-----------------------------------------------------
//                  WEB SOCKET AND LOGIC
//-----------------------------------------------------
//-----------------------------------------------------

void OssCore::startWebSocket()
{
    dnsServer->stop();
    DynamicJsonDocument config(1024);
    Tools::loadConfig(&config);
    Serial.println("[WSc] Connecting to Websocket (" + config["host"].as<String>() + ")...");
    webSocket.beginSSL(config["host"].as<String>(), WS_PORT);
    webSocket.onEvent(std::bind(&OssCore ::webSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    webSocket.setReconnectInterval(5000);

    // start heartbeat (optional)
    // ping server every 15000 ms
    // expect pong from server within 3000 ms
    // consider connection disconnected if pong is not received 2 times
    webSocket.enableHeartbeat(3000, 2000, 2);
}

void OssCore::webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{

    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[WSc] Disconnected!\n");
        break;

    case WStype_ERROR:
        Serial.printf("[WSc] ERROR!\n");
        Serial.println(String((char *)payload));
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
                    Serial.println("[WSc] Authentication success !");
                }
                else
                {
                    Serial.println("[WSc] Authentication fail, please check you'r token and uuid !");
                }
            }
            else if (received["type"] == "GET_STATE")
            {
                this->updateStateCallBack(&received);
                String str;
                serializeJson(stateObject, str);
                webSocket.sendTXT(str);
            }
            else if (received["type"] == "COMMAND")
            {
                this->commandCallBack(&received);
                this->updateStateCallBack(&received);
                String str;
                serializeJson(stateObject, str);
                webSocket.sendTXT(str);
                
            }
        }
        break;
    }
}

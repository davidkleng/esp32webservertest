
//#define DEBUG
#define INFO

#include <WiFi.h>
#include <ESPmDNS.h>

#include <FS.h>
#include <SPIFFS.h>

#include <ESPAsyncWebServer.h>

float readASD1115single(int op);
String readMODBUS(uint16_t id, uint16_t add, uint16_t qty);
String listDirectorySDCard();

AsyncWebServer server(80);

// 3 leds
const int led1Pin = 32;
bool led1State = false;
const int led2Pin = 25;
bool led2State = false;
const int led3Pin = 26;
bool led3State = false;

// pwm led
const int ledPin = 14;
int16_t pwmVal = 0;
// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

// potenciometer
const int potPin = 34;
float adcVal = 0;

void handleFormLed1(AsyncWebServerRequest *request)
{
    if (!request->authenticate(http_username, http_password))
        request->redirect("/index.html");

#ifdef DEBUG
    Serial.println("Params.......");
    int params = request->params();
    for (int i = 0; i < params; i++)
    {
        AsyncWebParameter *p = request->getParam(i);
        if (p->isFile())
        { //p->isPost() is also true
            Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
        }
        else if (p->isPost())
        {
            Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
        else
        {
            Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
    }

    //List all parameters (Compatibility)
    Serial.println("Args.............");
    int args = request->args();
    for (int i = 0; i < args; i++)
    {
        Serial.printf("ARG[%s]: %s\n", request->argName(i).c_str(), request->arg(i).c_str());
    }
#endif

    String led1 = request->arg("led1");

#ifdef INFO
    Serial.println(led1);
#endif

    led1State = led1.toInt();
    digitalWrite(led1Pin, led1State);

    request->send(200, "text/plain");
    //request->redirect("/");
}

void handleFormLed2(AsyncWebServerRequest *request)
{
    if (!request->authenticate(http_username, http_password))
        request->redirect("/index.html");

#ifdef DEBUG
    Serial.println("Params.......");
    int params = request->params();
    for (int i = 0; i < params; i++)
    {
        AsyncWebParameter *p = request->getParam(i);
        if (p->isFile())
        { //p->isPost() is also true
            Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
        }
        else if (p->isPost())
        {
            Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
        else
        {
            Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
    }

    //List all parameters (Compatibility)
    Serial.println("Args.............");
    int args = request->args();
    for (int i = 0; i < args; i++)
    {
        Serial.printf("ARG[%s]: %s\n", request->argName(i).c_str(), request->arg(i).c_str());
    }
#endif

    String led2 = request->arg("led2");

#ifdef INFO
    Serial.println(led2);
#endif

    led2State = led2.toInt();
    digitalWrite(led2Pin, led2State);

    request->send(200, "text/plain");
    //request->redirect("/");
}

void handleFormLed3(AsyncWebServerRequest *request)
{
    if (!request->authenticate(http_username, http_password))
        request->redirect("/index.html");

#ifdef DEBUG
    Serial.println("Params.......");
    int params = request->params();
    for (int i = 0; i < params; i++)
    {
        AsyncWebParameter *p = request->getParam(i);
        if (p->isFile())
        { //p->isPost() is also true
            Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
        }
        else if (p->isPost())
        {
            Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
        else
        {
            Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
    }

    //List all parameters (Compatibility)
    Serial.println("Args.............");
    int args = request->args();
    for (int i = 0; i < args; i++)
    {
        Serial.printf("ARG[%s]: %s\n", request->argName(i).c_str(), request->arg(i).c_str());
    }
#endif

    String led3 = request->arg("led3");

#ifdef INFO
    Serial.println(led3);
#endif

    led3State = led3.toInt();
    digitalWrite(led3Pin, led3State);

    request->send(200, "text/plain");
    //request->redirect("/");
}

void handleFormPot(AsyncWebServerRequest *request)
{
    if (!request->authenticate(http_username, http_password))
        request->redirect("/index.html");

    adcVal = readASD1115single(1);

#ifdef INFO
    Serial.println("Handle ADC");
    Serial.println(adcVal);
#endif

    request->send(200, "text/plain", String(adcVal));
    //request->redirect("/");
}

void handleFormMODBUS(AsyncWebServerRequest *request)
{
#ifdef DEBUG
    Serial.println("Params.......");
    int params = request->params();
    for (int i = 0; i < params; i++)
    {
        AsyncWebParameter *p = request->getParam(i);
        if (p->isFile())
        { //p->isPost() is also true
            Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
        }
        else if (p->isPost())
        {
            Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
        else
        {
            Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
    }

    //List all parameters (Compatibility)
    Serial.println("Args.............");
    int args = request->args();
    for (int i = 0; i < args; i++)
    {
        Serial.printf("ARG[%s]: %s\n", request->argName(i).c_str(), request->arg(i).c_str());
    }
#endif

    String id = request->arg("id");
    String add = request->arg("address");
    String qty = request->arg("quantity");

#ifdef INFO
    Serial.println("Handle MODBUS");
    Serial.println(id);
    Serial.println(add);
    Serial.println(qty);
#endif
    String msg;
    if (id.toInt() == 99)
    {
        msg += "123";
        msg += ",";
        msg += "123";
        msg += ",";
        msg += "123";
        msg += ",";
        msg += "123";
        msg += ",";
        msg += "123";
        msg += ",";
        msg += "123";
        msg += ",";
        msg += "123";
    }
    else
    {
        msg = readMODBUS(id.toInt(), add.toInt(), qty.toInt());
    }

    request->send(200, "text/plain", msg);
    // request->redirect("/");
}

void handleFormLed(AsyncWebServerRequest *request)
{
    if (!request->authenticate(http_username, http_password))
        request->redirect("/index.html");

#ifdef DEBUG
    Serial.println("Params.......");
    int params = request->params();
    for (int i = 0; i < params; i++)
    {
        AsyncWebParameter *p = request->getParam(i);
        if (p->isFile())
        { //p->isPost() is also true
            Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
        }
        else if (p->isPost())
        {
            Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
        else
        {
            Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
    }

    //List all parameters (Compatibility)
    Serial.println("Args.............");
    int args = request->args();
    for (int i = 0; i < args; i++)
    {
        Serial.printf("ARG[%s]: %s\n", request->argName(i).c_str(), request->arg(i).c_str());
    }
#endif

    String PWM = request->arg("pwm");
    pwmVal = PWM.toInt();

#ifdef INFO
    Serial.println(pwmVal);
#endif

    ledcWrite(ledChannel, pwmVal);

    //request->redirect("/login.html");
    request->send(200, "text/plain");
}

void handleFormStates(AsyncWebServerRequest *request)
{
    adcVal = readASD1115single(1);

    String msg = "";
    msg += String(led1State);
    msg += ",";
    msg += String(led2State);
    msg += ",";
    msg += String(led3State);
    msg += ",";
    msg += String(pwmVal);
    msg += ",";
    msg += String(adcVal);
    request->send(200, "text/plain", msg);
    // request->redirect("/");
}

void handleFormFiles(AsyncWebServerRequest *request)
{
    String msg = listDirectorySDCard();
    request->send(200, "text/plain", msg);
}

// HANDLE REDIRECTS

void handleRoot(AsyncWebServerRequest *request)
{
#ifdef INFO
    Serial.println("Handle Root");
#endif

    if (!request->authenticate(http_username, http_password))
        return request->requestAuthentication();

    //
    request->redirect("/index.html");
    request->send(200, "text/html");
}

void handleLogout(AsyncWebServerRequest *request)
{
#ifdef INFO
    Serial.println("Handle Logout");
#endif
    request->send(401);
    request->redirect("/index.html");
}

void handleLogs(AsyncWebServerRequest *request)
{
#ifdef INFO
    Serial.println("Handle Logs");
#endif

    if (!request->authenticate(http_username, http_password))
        return request->requestAuthentication();

    //
    request->redirect("/html/logs.html");
    request->send(200, "text/html");
}

void InitServer()
{
    // server.serveStatic("/", SPIFFS, "/");
    server.serveStatic("/", SPIFFS, "/").setDefaultFile("");
    server.serveStatic("/html/logs.html", SPIFFS, "/html/logs.html").setDefaultFile("html/logs.html");

    server.serveStatic("/exe", SPIFFS, "/login.html").setDefaultFile("login.html");

    //server
    //     .serveStatic("/", SPIFFS, "/")
    //     .setDefaultFile("index.html");

    // get current states at begining
    server.on("/states", HTTP_GET, handleFormStates);

    server.on("/led1", HTTP_POST, handleFormLed1);
    server.on("/led2", HTTP_POST, handleFormLed2);
    server.on("/led3", HTTP_POST, handleFormLed3);
    server.on("/adc", HTTP_GET, handleFormPot);
    server.on("/pwm", HTTP_POST, handleFormLed);
    server.on("/logout", HTTP_GET, handleLogout);
    server.on("/modbus", HTTP_GET, handleFormMODBUS);
    server.on("/files", HTTP_GET, handleFormFiles);

    // Route for root / web page
    /*
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  if (!request->authenticate(http_username, http_password))
                      return request->requestAuthentication();
                  request->redirect("/index.html");
              });
    */
    // Route for root / web page
    server.on("/", HTTP_GET, handleRoot);
    server.on("/index", HTTP_GET, handleRoot);
    server.on("/index.html", HTTP_GET, handleRoot);
    server.on("/html/logs.html", HTTP_GET, handleLogs);

    server.onNotFound([](AsyncWebServerRequest *request)
                      { request->send(400, "text/plain", "Not found"); });

    server.begin();

    Serial.println("HTTP server started");
}

void ConnectWiFi_STA(bool useStaticIP = false)
{
    Serial.println("");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (useStaticIP)
        WiFi.config(ip, gateway, subnet);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.print('.');
    }

    Serial.println("");
    Serial.print("Iniciado STA:\t");
    Serial.println(ssid);
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());
}

void ConnectWiFi_AP(bool useStaticIP = false)
{
    Serial.println("");
    WiFi.mode(WIFI_AP);
    while (!WiFi.softAP(ssid, password))
    {
        Serial.println(".");
        delay(100);
    }
    if (useStaticIP)
        WiFi.softAPConfig(ip, gateway, subnet);

    Serial.println("");
    Serial.print("Iniciado AP:\t");
    Serial.println(ssid);
    Serial.print("IP address:\t");
    Serial.println(WiFi.softAPIP());
}

void InitMDNS()
{
    if (!MDNS.begin(hostname))
    {
        Serial.println("Error iniciando mDNS");
    }

    MDNS.addService("http", "tcp", 80);

    Serial.println("mDNS iniciado");
}
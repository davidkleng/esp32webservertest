#include <Arduino.h>

#include "config.h" // Sustituir con datos de red
#include "Server.hpp"
#include "devices.hpp"
#include "ESP32_Utils_OTA.hpp"

#define LED_BUILTIN 1

/*
void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}
*/

void setup(void)
{
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);

  ledcWrite(ledChannel, 255);
  digitalWrite(led1Pin, HIGH);
  digitalWrite(led2Pin, HIGH);
  digitalWrite(led3Pin, HIGH);

  Serial.begin(9600);

  //serial 1 y 2 for modbus
  //Serial1.begin(9600, SERIAL_8N1, 0, 4); // not working on default pins
  Serial2.begin(9600, SERIAL_8N1);

  SPIFFS.begin();

  // setup devices
  setupADS1115();
  setupHW125();

  //ConnectWiFi_AP(); // using aldo's wifi for testing, then switching to AP mode
  ConnectWiFi_STA();

  InitOTA();

  InitServer();

  InitMDNS();

  delay(1000);
  ledcWrite(ledChannel, 0);
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);
  digitalWrite(led3Pin, LOW);
}

void loop(void)
{
  ArduinoOTA.handle();
  //  String msg = "";
  //  msg += String(millis());
  //  msg += "\r\n";
  // logSDCard(msg);
  delay(2000);
}

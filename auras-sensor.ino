#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoOTA.h>

#include <Adafruit_NeoPixel.h>

#include "parameters.h"
#include "auras-sensor.h"
#include "Light.h"

Light mLight;
bool bUpdateTouch = false;

int lightValue = LOW;
int touchValue = LOW;

void setup() {
  Serial.begin(115200);
  Serial.println("\nSetup");
  pinMode(2, OUTPUT);

  nextTouchUpdate = millis() + TOUCH_UPDATE_PERIOD_MILLIS;
  nextLightUpdate = millis() + LIGHT_UPDATE_PERIOD_MILLIS;

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());

  for (int counter = 0; (WiFi.status() != WL_CONNECTED) && (counter < 32); counter++) {
    delay(500);
  }

  ID = getIdFromMacAddress(WiFi.macAddress());
  OTA_HOSTNAME += ID;

  setupAndStartOTA();
}

void updateLight() {
  HTTPClient http;
  http.begin("http://" + SERVER_ADDRESS + ":" + SERVER_PORT + LIGHT_ENDPOINT);
  int httpCode = http.GET();
  delay(10);

  if (httpCode == HTTP_CODE_OK) {
    lightValue = http.getString().toInt();
  }
  http.end();
  lightValue = (int)random(0, 2);
  mLight.setColor((float)lightValue);
}

void updateTouch() {
  // TODO: read sensor
  touchValue = (int)random(0, 2);
  HTTPClient http;
  http.begin("http://" + SERVER_ADDRESS + ":" + SERVER_PORT + TOUCH_ENDPOINT);
  int httpCode = http.GET();
  delay(10);
  http.end();
}

void loop() {
  if (bUpdateTouch && (millis() > nextTouchUpdate)) {
    if (WiFi.status() == WL_CONNECTED) updateTouch();
    nextTouchUpdate += TOUCH_UPDATE_PERIOD_MILLIS;
  }

  if ((millis() > nextLightUpdate)) {
    if (WiFi.status() == WL_CONNECTED) updateLight();
    nextLightUpdate += LIGHT_UPDATE_PERIOD_MILLIS;
  }

  digitalWrite(2, (nextTouchUpdate / TOUCH_UPDATE_PERIOD_MILLIS) % 2);
  ArduinoOTA.handle();
}


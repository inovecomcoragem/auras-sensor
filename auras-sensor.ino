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

short PIR_PIN = D2;

void setup() {
  Serial.begin(115200);
  Serial.println("\nSetup");
  pinMode(D4, OUTPUT);
  pinMode(PIR_PIN, INPUT);

  mLight.setColor(0.0f);
  nextLightUpdate = millis() + LIGHT_UPDATE_PERIOD_MILLIS;

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());

  for (int counter = 0; (WiFi.status() != WL_CONNECTED) && (counter < 32); counter++) {
    delay(800);
    Serial.println("Trying WiFi");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected");
  }

  OTA_HOSTNAME += WiFi.macAddress().substring(12);

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
  Serial.println(lightValue);
  if (lightValue) {
    mLight.setColor(0.5f);
    bUpdateTouch = true;
  }
}

void updateTouch() {
  for (int i = 0; i < 4; i++) {
    touchValue = digitalRead(PIR_PIN);
    Serial.println(touchValue);
    if (!touchValue) return;
    delay(100);
  }

  HTTPClient http;
  http.begin("http://" + SERVER_ADDRESS + ":" + SERVER_PORT + TOUCH_ENDPOINT + "/1");
  int httpCode = http.GET();
  delay(10);
  http.end();
  delay(4000);
  mLight.setColor(0.0f);
  bUpdateTouch = false;
}

void loop() {
  if ((!bUpdateTouch) && (millis() > nextLightUpdate)) {
    if (WiFi.status() == WL_CONNECTED) updateLight();
    nextLightUpdate += LIGHT_UPDATE_PERIOD_MILLIS;
  }

  if (bUpdateTouch) {
    if (WiFi.status() == WL_CONNECTED) updateTouch();
  }

  digitalWrite(D4, (nextLightUpdate / LIGHT_UPDATE_PERIOD_MILLIS) % 2);
  ArduinoOTA.handle();
}


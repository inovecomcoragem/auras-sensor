#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoOTA.h>
#include <Ticker.h>

#include <Adafruit_NeoPixel.h>

#include "parameters.h"
#include "auras-sensor.h"
#include "Light.h"
#include "SensorPin.h"

short LED_PIN = D4;
short SENSOR_PIN = A0;

Light mLight(PIXELS_PIN, NUMPIXELS);
SensorPin mSensor(SENSOR_PIN);
Ticker mTicker;

bool bUpdateTouch = false;

int lightValue = LOW;
int touchValue = LOW;

void mSensorSampleWrapper() {
  mSensor.getSample();
}

void setup() {
  Serial.begin(115200);
  Serial.println("\nSetup");
  pinMode(LED_PIN, OUTPUT);

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
  mTicker.attach_ms(10, mSensorSampleWrapper);
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

  Serial.println(lightValue);
  if (lightValue) {
    mLight.setColor(1.0f);
    bUpdateTouch = true;
  }
}

void updateTouch() {
  for (int i = 0; i < 4; i++) {
    touchValue = mSensor.getReading();
    Serial.println(touchValue);
    if (!touchValue) return;
    delay(100);
  }

  HTTPClient http;
  http.begin("http://" + SERVER_ADDRESS + ":" + SERVER_PORT + TOUCH_ENDPOINT + "/1");
  http.GET();
  delay(10);
  http.end();
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

  digitalWrite(LED_PIN, (nextLightUpdate / LIGHT_UPDATE_PERIOD_MILLIS) % 2);
  ArduinoOTA.handle();
}


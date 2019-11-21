#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Ticker.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include <Adafruit_NeoPixel.h>

#include "parameters.h"
#include "auras-sensor.h"
#include "Light.h"
#include "SensorPin.h"

Light mLight(PIXELS_PIN, NUMPIXELS);
SensorPin mSensor(SENSOR_PIN);
Ticker mTicker;

int lightValue[] = { LOW, LOW };
int touchValue = LOW;
int touchRaw, touchAverage;

void mSensorSampleWrapper() {
  mSensor.getSample();
}

void setup() {
  Serial.begin(115200);
  Serial.println("\nSetup");
  pinMode(LED_PIN, OUTPUT);

  WiFiManager wifiManager;
  // wifiManager.resetSettings();
  wifiManager.setAPStaticIPConfig(IPAddress(1, 2, 3, 4), IPAddress(1, 2, 3, 4), IPAddress(255, 255, 255, 0));
  wifiManager.autoConnect("AURAS-WIFI-SETUP");

  mLight.setColor(0.0f);
  nextUpdate = millis() + UPDATE_PERIOD_MILLIS;

  setupAndStartOTA(SERVER_ADDRESS);

  mTicker.attach_ms(10, mSensorSampleWrapper);
}

int getLight() {
  if (WiFi.status() != WL_CONNECTED) return lightValue[0];

  HTTPClient http;
  http.begin(SERVER_ADDRESS + ":" + SERVER_PORT + LIGHT_ENDPOINT);
  int httpCode = http.GET();
  delay(10);

  if (httpCode == HTTP_CODE_OK) {
    lightValue[1] = lightValue[0];
    lightValue[0] = http.getString().toInt();
  }
  http.end();

  String printString = "From getLight: lightValue = " + String(lightValue[0]);
  Serial.println(printString);

  return lightValue[0];
}

void setTouch(int touchVal) {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.begin(SERVER_ADDRESS + ":" + SERVER_PORT + TOUCH_ENDPOINT + "/" + String(touchVal));
  http.GET();
  delay(10);
  http.end();
}

void updateTouch() {
  for (int i = 0; i < 4; i++) {
    touchValue = mSensor.getReading();
    touchRaw = mSensor.getReadingRaw();
    touchAverage = mSensor.getSlowAverage();

    String printString = "From updateTouch: = " + String(touchRaw);
    printString += " <-> " + String(touchAverage) + " = ";
    printString += String(touchValue);
    Serial.println(printString);

    delay(100);
    if (!touchValue) break;
  }
  setTouch(touchValue);
}

void loop() {
  if (millis() > nextUpdate) {
    updateTouch();
    lightValue[0] = getLight();
    mLight.setColor((float)lightValue[0]);
    nextUpdate += UPDATE_PERIOD_MILLIS;
  }

  digitalWrite(LED_PIN, (nextUpdate / UPDATE_PERIOD_MILLIS) % 2);
  ArduinoOTA.handle();
}

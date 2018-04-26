#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Ticker.h>

#include <Adafruit_NeoPixel.h>

#include "parameters.h"
#include "auras-sensor.h"
#include "auras-server.h"
#include "Light.h"
#include "SensorPin.h"

Light mLight(PIXELS_PIN, NUMPIXELS);
SensorPin mSensor(SENSOR_PIN);
Ticker mTicker;

int touchRaw, touchAverage;

void mSensorSampleWrapper() {
  mSensor.getSample();
}

void setup() {
  Serial.begin(115200);
  Serial.println("\nSetup");
  pinMode(LED_PIN, OUTPUT);

  mLight.setColor(0.0f);
  nextUpdate = millis() + UPDATE_PERIOD_MILLIS;

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());

  for (int counter = 0; (WiFi.status() != WL_CONNECTED) && (counter < 32); counter++) {
    delay(800);
    Serial.println("Trying WiFi");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  }

  setRoutesAndStartServer();
  setupAndStartOTA(SERVER_ADDRESS);

  mTicker.attach_ms(10, mSensorSampleWrapper);
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
}

void loop() {
  if (millis() > nextUpdate) {
    updateTouch();

    if (lightValue[0] != lightValue[1]) {
      mLight.setColor((float)lightValue[0]);
    }

    nextUpdate += UPDATE_PERIOD_MILLIS;
  }

  digitalWrite(LED_PIN, (nextUpdate / UPDATE_PERIOD_MILLIS) % 2);
  mServer.handleClient();
  ArduinoOTA.handle();
}


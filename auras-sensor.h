const short PIXELS_PIN = D1;
const short LED_PIN = D4;
const short SENSOR_PIN = A0;

const short NUMPIXELS = 60;

String TOUCH_ENDPOINT = "/set-touch";
String LIGHT_ENDPOINT = "/get-light";

String OTA_HOSTNAME = "Auras-";

long UPDATE_PERIOD_MILLIS = 1000;
long nextUpdate = 0L;

void setupAndStartOTA() {
  ArduinoOTA.setHostname(OTA_HOSTNAME.c_str());
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}


String TOUCH_ENDPOINT = "/touch";
String LIGHT_ENDPOINT = "/light";

String OTA_HOSTNAME = "Auras-";
String ID = "";

long TOUCH_UPDATE_PERIOD_MILLIS = 5e3L;
long nextTouchUpdate = 0L;

long LIGHT_UPDATE_PERIOD_MILLIS = 5e3L;
long nextLightUpdate = 0L;

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

String getIdFromMacAddress(String mac) {
  if (mac.indexOf("2C:3A:E8:1C:C2:E7") != -1) return  "1";
  else return "0";
}


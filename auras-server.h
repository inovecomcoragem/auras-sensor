ESP8266WebServer mServer(SERVER_PORT);

int lightValue[] = { LOW, LOW };
int touchValue = LOW;

void handleLightOn() {
  lightValue[1] = lightValue[0];
  lightValue[0] = 1;
  mServer.send(200, "text/plain", "light on");
}

void handleLightOff() {
  lightValue[1] = lightValue[0];
  lightValue[0] = 0;
  mServer.send(200, "text/plain", "light off");
}

void handleGetTouch() {
  mServer.send(200, "text/plain", String(touchValue));
}

void handleGetLight() {
  mServer.send(200, "text/plain", String(lightValue[0]));
}

void handleNotFound() {
  String message = "404\n" + mServer.uri() + "\nNot Found\n";
  mServer.send(404, "text/plain", message);
}

void setRoutesAndStartServer() {
  mServer.on("/get-touch", handleGetTouch);
  mServer.on("/get-light", handleGetLight);
  mServer.on("/set-light/0", handleLightOff);
  mServer.on("/set-light/1", handleLightOn);
  mServer.onNotFound(handleNotFound);
  mServer.begin();
}


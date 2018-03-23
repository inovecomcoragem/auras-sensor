## Auras: Sensor

Code for the [NodeMCU](https://nodemcu.readthedocs.io/en/master/) module that controls the crystal ball.

---

1. **Clone repository:**
```
git clone https://github.com/thiagohersan/auras-sensor.git
```

2. **Copy** ```parameters.h.example``` **to** ```parameters.h``` **and change values for WiFi network name, WiFi password, app server address:port:**
```
String WIFI_SSID = "my-network";
String WIFI_PASS = "my-password";
```
```
String SERVER_ADDRESS = "127.0.0.1";
int SERVER_PORT = 8000;
```

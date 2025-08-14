#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DHT11.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ESP8266WebServer.h>

#define BUADRATE 115200
#define IRK_GPIO_PORT 12
#define DHT_GPIO_PORT 14
#define WEB_SERVER_PORT 80
#define WEB_JSON_HEADER "application/json"

const char *SSID = "WiFi";
const char *Password = "7v0G84fUvDPeAwG";
const uint16_t ACOn[229] = { 3056, 3054, 3058, 4430, 568, 1682, 548, 554, 570, 1680, 548, 560, 570, 554, 570, 1682, 546, 1682, 548, 560, 570, 1680, 548, 554, 572, 552, 572, 558, 570, 1680, 548, 1680, 548, 554, 570, 558, 572, 554, 572, 552, 572, 1680, 550, 1670, 548, 552, 572, 552, 572, 552, 572, 1670, 550, 552, 572, 552, 572, 550, 574, 556, 572, 552, 572, 552, 572, 552, 572, 558, 574, 550, 574, 1678, 550, 552, 572, 556, 574, 1678, 550, 552, 572, 552, 572, 1670, 548, 1680, 550, 552, 572, 1680, 548, 558, 570, 554, 570, 554, 570, 554, 568, 560, 570, 554, 570, 556, 570, 554, 570, 560, 570, 556, 568, 556, 568, 556, 570, 560, 568, 556, 568, 556, 570, 1680, 548, 562, 568, 556, 570, 556, 568, 556, 570, 560, 568, 556, 568, 556, 568, 556, 568, 562, 568, 554, 570, 556, 568, 556, 568, 560, 570, 556, 568, 556, 568, 556, 568, 562, 568, 556, 568, 556, 568, 558, 568, 562, 568, 556, 568, 558, 566, 558, 566, 562, 568, 556, 566, 558, 566, 558, 568, 562, 566, 558, 568, 558, 566, 558, 568, 562, 568, 556, 566, 556, 568, 556, 568, 562, 568, 556, 570, 556, 568, 556, 568, 562, 568, 556, 568, 1660, 568, 556, 568, 1650, 568, 556, 568, 1662, 568, 1662, 568, 1652, 568, 556, 568, 556, 568, 556, 568, 1646, 570 };
const uint16_t ACOff[229] = { 3086, 3056, 3056, 4432, 566, 1682, 546, 558, 566, 1684, 544, 564, 566, 558, 566, 1684, 544, 1684, 546, 564, 566, 1682, 546, 558, 566, 558, 566, 564, 566, 1684, 546, 1684, 544, 560, 566, 562, 566, 558, 566, 558, 566, 1684, 546, 1674, 546, 558, 566, 558, 566, 558, 568, 1674, 546, 558, 566, 558, 568, 558, 568, 562, 568, 558, 566, 558, 568, 556, 568, 562, 568, 556, 568, 556, 566, 558, 568, 562, 568, 1682, 546, 556, 568, 556, 568, 1672, 548, 1682, 548, 556, 570, 1680, 548, 560, 568, 556, 568, 556, 568, 556, 570, 560, 568, 556, 570, 556, 568, 556, 570, 560, 570, 556, 570, 556, 570, 554, 570, 560, 570, 556, 568, 556, 570, 1682, 548, 560, 570, 554, 570, 554, 570, 556, 570, 560, 570, 554, 570, 554, 570, 554, 572, 558, 570, 552, 572, 554, 570, 554, 570, 558, 570, 554, 572, 554, 572, 552, 572, 558, 572, 554, 570, 554, 570, 552, 572, 558, 572, 552, 572, 554, 572, 552, 572, 558, 570, 554, 572, 552, 572, 554, 570, 558, 572, 554, 572, 552, 572, 554, 570, 558, 572, 554, 570, 554, 572, 554, 572, 558, 572, 552, 572, 552, 572, 554, 572, 558, 572, 552, 572, 1680, 550, 552, 572, 1670, 550, 552, 572, 552, 572, 1680, 550, 1670, 550, 552, 572, 552, 572, 552, 572, 1644, 570 };

IRsend IR(IRK_GPIO_PORT);
DHT11 DHT(DHT_GPIO_PORT);
ESP8266WebServer Server(WEB_SERVER_PORT);

void log(String message) {
  unsigned long milliseconds = millis();
  unsigned long seconds = milliseconds / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;

  int hour = hours % 100;  // Reset to 0 after 100 hours
  int minute = minutes % 60;
  int second = seconds % 60;

  // Format the timestamp
  char timestamp[11];
  snprintf(timestamp, sizeof(timestamp), "[%02d:%02d:%02d]",
           hour, minute, second);

  // Combine and print the log message
  Serial.println(String(timestamp) + ": " + message);
}

void wlog(int code) {
  String method = (Server.method() == HTTP_GET) ? "GET" : "POST";
  String message = String(code) + " - " + method + " " + Server.uri();
  log(message);
}

String jsonify(int n, const char *key, ...) {
    String json = "{";
    va_list args;
    va_start(args, key);
    int index = 0;
    while (index < n) {
        const char *value = va_arg(args, const char*);
        if (!value) break;
        if (index) json += ",";
        json += "\"" + String(key) + "\":\"" + String(value) + "\"";
        key = va_arg(args, const char*);
        index++;
    }
    va_end(args);
    json += "}";
    return json;
}

void measure() {
  int temperature = 0;
  int humidity = 0;
  int result = DHT.readTemperatureHumidity(temperature, humidity);

  // Check if measurement successful
  if (result == 0) {
    Server.send(200, WEB_JSON_HEADER,
                jsonify(2, "temprature", String(temperature), "humidity", String(humidity)));
    wlog(200);
  } else {
    Server.send(400, WEB_JSON_HEADER, jsonify(1, "message", DHT11::getErrorString(result)));
    wlog(400);
  }
}

void power() {
  // If request not contains arg 'status'
  if (!Server.hasArg("status")) {
    Server.send(400, WEB_JSON_HEADER, jsonify(1, "message", "arg 'status' is required"));
    wlog(400);
    return;
  }

  if (Server.arg("status") == "on") {
    IR.sendRaw(ACOn, sizeof(ACOn) / sizeof(ACOn[0]), 38);
    Server.send(200, WEB_JSON_HEADER, jsonify(1, "status", "on"));
    wlog(200);
    return;
  }
  if (Server.arg("status") == "off") {
    IR.sendRaw(ACOff, sizeof(ACOff) / sizeof(ACOff[0]), 38);
    Server.send(200, WEB_JSON_HEADER, jsonify(1, "status", "off"));
    wlog(200);
    return;
  }
  Server.send(400, WEB_JSON_HEADER, jsonify(1, "message", "arg 'status' should be 'on' or 'off'"));
  wlog(400);
}

void notfound() {
  Server.send(404, WEB_JSON_HEADER, "{}");
  wlog(404);
}

void setup() {
  Serial.begin(BUADRATE);

  // Connect to AP
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, Password);
  Serial.println();
  log(String("connecting to AP: ") + SSID);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Show connection
  Server.begin();
  log(String("listening address: ") + WiFi.localIP().toString() + ":" + WEB_SERVER_PORT);

  // Bind addresses
  Server.onNotFound(notfound);
  Server.on("/measure", measure);
  Server.on("/power", power);

  // Starting IR device
  IR.begin();
}

void loop() {
  Server.handleClient();
}

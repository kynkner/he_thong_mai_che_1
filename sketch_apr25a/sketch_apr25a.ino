#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Realme";
const char* password = "123456789@";
const char* serverUrl = "http://192.168.1.34:5000//data";

const int rainSensorPin = 34; // Analog pin
const int lightSensorPin = 35;

const int motorPin1 = 14;
const int motorPin2 = 27;

// void setup() {
//   Serial.begin(115200);

//   pinMode(motorPin1, OUTPUT);
//   pinMode(motorPin2, OUTPUT);

//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("Connected to WiFi");
// }

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  // Gửi thử dữ liệu mẫu
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;

    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<200> doc;
    doc["timestamp"] = millis();
    doc["rain"] = true;
    doc["light"] = false;
    doc["roof"] = "open";

    String jsonData;
    serializeJson(doc, jsonData);

    int httpResponseCode = http.POST(jsonData);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    http.end();
  }
}

void loop() {
  int rainValue = analogRead(rainSensorPin);
  int lightValue = analogRead(lightSensorPin);

  bool isRaining = rainValue < 3000;     // tùy chỉnh ngưỡng
  bool isLight = lightValue > 1500;      // tùy chỉnh ngưỡng

  String roofStatus = "closed";
  if (isRaining || isLight) {
    openRoof();
    roofStatus = "open";
  } else {
    closeRoof();
    roofStatus = "closed";
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    String payload = "{";
    payload += "\"rain\":" + String(isRaining ? "true" : "false") + ",";
    payload += "\"light\":" + String(isLight ? "true" : "false") + ",";
    payload += "\"roof\":\"" + roofStatus + "\"";
    payload += "}";

      int httpResponseCode = http.POST(payload);
Serial.print("HTTP Response code: ");
Serial.println(httpResponseCode);

    http.POST(payload);
    http.end();
  }


  delay(10000); // gửi mỗi 10 giây
}

void openRoof() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
}

void closeRoof() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
}

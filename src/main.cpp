// https :  //randomnerdtutorials.com/esp32-ds18b20-temperature-arduino-ide/
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
// #include <Arduino_JSON.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 33;
const int LED_32 = 32;
// const int LED_BUILTIN = 2;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

void connectToWifi()
{
  const char *ssid = "new-new-internet";
  const char *password = "mileymo19";
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nConnected");
  Serial.print("ip address: ");
  Serial.println(WiFi.localIP());
}

void httpPost(float tempF)
{
  // get chip id
  String chipId = String((uint32_t)ESP.getEfuseMac(), HEX);

  // Prepare JSON document
  DynamicJsonDocument doc(2048);
  doc["chipName"] = "esp32";
  doc["chipId"] = chipId.c_str();
  doc["tempF"] = tempF;

  // Serialize JSON document
  String json;
  serializeJson(doc, json);

  // HTTP
  WiFiClient client; // or WiFiClientSecure for HTTPS
  HTTPClient http;

  // Send request
  http.begin(client, "http://192.168.68.142:3000/api/temperature");
  http.addHeader("Content-Type", "application/json");
  http.POST(json);

  // Read response
  Serial.println(http.getString());

  // Disconnect
  http.end();
}

void setup()
{
  Serial.begin(115200);
  connectToWifi();
  // Start the DS18B20 sensor
  sensors.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_32, OUTPUT);
}

void loop()
{
  if ((WiFi.status() == WL_CONNECTED))
  {
    sensors.requestTemperatures();
    float tempF = sensors.getTempFByIndex(0);
    Serial.println("TempF: " + String(tempF) + "ºF");
    httpPost(tempF);
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(LED_32, LOW);
  }
  else
  {
    Serial.println("offline");
  }

  delay(1000);
  Serial.println("---");
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LED_32, HIGH);
  delay(1000);
}

#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

// Wifi Credentials
const char *ssid = "new-new-internet"; // Wifi SSID
const char *password = "mileymo19";    // Wi-FI Password
WebSocketsClient webSocket;            // websocket client class instance
StaticJsonDocument<100> doc;           // Allocate a static JSON document

// int LED_BUILTIN = 32;

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  Serial.println("webSocketEvent type: " + String(type));

  if (type == WStype_TEXT)
  {
    DeserializationError error = deserializeJson(doc, payload); // deserialize incoming Json String
    if (error)
    { // Print erro msg if incoming String is not JSON formatted
      Serial.println("deserializeJson() failed: " + String(error.c_str()));
      return;
    }

    const boolean isOn = doc["isOn"];
    Serial.println("isOn: " + String(isOn));
    if (isOn == true)
    {
      Serial.println("true");
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
      Serial.println("false");
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}

void setup()
{
  // Connect to local WiFi
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Print local IP address
  delay(2000);                    // wait for 2s

  // address, port, and URL path
  webSocket.begin("192.168.68.142", 3001, "/heaterGpioState");
  // webSocket event handler
  webSocket.onEvent(webSocketEvent);
  // if connection failed retry every 5s
  webSocket.setReconnectInterval(5000);
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop()
{
  webSocket.loop(); // Keep the socket alive
}

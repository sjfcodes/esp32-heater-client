#include "wsHeater.h"
#include "wifiConnect.h"
#include <ArduinoJson.h>
#include <WebServer.h>
#include <WebSocketsClient.h>

// GPIO
const int thermoPin = 33; // Thermostat GPIO pin

// Websocket server location
const int wsServerPort = 3001;                 // Websocket server port
const char *wsServerIp = "192.168.68.142";     // Websocket server's host ip
const char *wsServerPath = "/heaterGpioState"; // Websocket server path
WebSocketsClient webSocket;                    // Websocket client instance
StaticJsonDocument<100> doc;                   // Allocate static JSON document

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  Serial.println("webSocketEvent type: " + String(type));
  if (type == WStype_TEXT)
  {
    DeserializationError error = deserializeJson(doc, payload); // deserialize incoming Json String
    if (error)
    { // Print erro msg if incoming String is not JSON formatted
      Serial.println("JSON DeserializationError: " + String(error.c_str()));
      return;
    }

    const boolean isOn = doc["isOn"];
    Serial.println("isOn: " + String(isOn));
    if (isOn == true)
      heaterOn();
    else
      heaterOff();
  }
  Serial.println("---");
}

void setup()
{
  Serial.begin(115200);
  wifiConnect();
  webSocket.begin(wsServerIp, wsServerPort, wsServerPath); // Connect to ws server
  webSocket.onEvent(webSocketEvent);                       // Handle ws events
  webSocket.setReconnectInterval(5000);                    // Handle connection failure, retry every 5s
  heaterSetup();                                           // Init pin GPIO pin
  Serial.println("---");
}

void loop()
{
  webSocket.loop(); // Keep the socket alive
}

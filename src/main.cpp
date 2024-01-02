#include "heater.h"
#include "sensor.h"
#include "myTime.h"

#include "wifiConnect.h"
#include <ArduinoJson.h>
#include <WebServer.h>
#include <WebSocketsClient.h>

// Websocket server location
const int wsServerPort = 3001;              // Websocket server port
const char *wsServerIp = "192.168.68.142";  // Websocket server's host ip
const char *wsServerPath = "/heater-cab-0"; // Websocket server path
WebSocketsClient webSocket;                 // Websocket client instance
StaticJsonDocument<100> doc;                // Allocate static JSON document

unsigned long epochTime;     // Variable to save current epoch time
unsigned long nextEpochTime; // Variable to save next epoch time

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
  sensorSetup();
  setupTime();
  nextEpochTime = getTime() + 2;
  Serial.println("---");
}

void loop()
{
  webSocket.loop(); // Keep the socket alive

  /**
   * using a delay(5000) in the loop
   * caused the websocket connection to fail
   * as a workaround, measure epoch on each iteration
   * broadcast new data when enough time passes
   */
  epochTime = getTime();
  if (epochTime == nextEpochTime)
  {
    Serial.println("Epoch Time: " + String(epochTime));
    nextEpochTime = epochTime + 5;

    float cabHumidity = getHumidity();
    float cabTempF = getTemperature();

    webSocket.sendTXT("{\"cabHumidity\":" + String(cabHumidity) + ",\"cabTempF\":" + String(cabTempF) + ",\"updatedAt\":" + String(epochTime) + "}");
    Serial.println("{\"cabHumidity\":" + String(cabHumidity) + ",\"cabTempF\":" + String(cabTempF) + ",\"updatedAt\":" + String(epochTime) + "}");
    Serial.println("---");
  }
}

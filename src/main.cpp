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
StaticJsonDocument<100> JsonIn;             // Allocate static JSON document

const short loopIntervalSec = 4; // Seconds between each loop
unsigned long epochTime;         // Current epoch time
unsigned long nextEpochTime;     // Next epoch time
unsigned long offlineAt;         // Track time websocket goes offline
const short resetAfterOfflineForSec = 30;
String chipId;

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  Serial.println("webSocketEvent type: " + String(type));
  if (type == WStype_DISCONNECTED)
  {
    unsigned long now = getTime();
    if (!offlineAt)
    {
      // set offlineAt
      offlineAt = now;
      Serial.println("Set offlineAt: " + String(offlineAt));
    }
    // if offline for xx seconds, restart & attempt reconnect
    else if (now - offlineAt > resetAfterOfflineForSec)
    {
      Serial.println("Restarting at: " + String(now));
      ESP.restart();
    }
  }
  if (type == WStype_TEXT)
  {
    DeserializationError error = deserializeJson(JsonIn, payload); // deserialize incoming Json String
    if (error)
    { // Print erro msg if incoming String is not JSON formatted
      Serial.println("JSON DeserializationError: " + String(error.c_str()));
      return;
    }

    const boolean heaterPinVal = JsonIn["heaterPinVal"];
    Serial.println("heaterPinVal: " + String(heaterPinVal));
    if (heaterPinVal == true)
      heaterOn();
    else
      heaterOff();
  }
  Serial.println("---");
}

void setup()
{
  Serial.begin(115200);
  chipId = String((uint32_t)ESP.getEfuseMac(), HEX);       // Set chip id
  wifiConnect();                                           // Connect to wifi
  webSocket.begin(wsServerIp, wsServerPort, wsServerPath); // Connect to ws server
  webSocket.onEvent(webSocketEvent);                       // Handle ws events
  webSocket.setReconnectInterval(loopIntervalSec * 1000);  // Handle connection failure, retry every 5s
  pinMode(LED_BUILTIN, OUTPUT);
  heaterSetup();
  sensorSetup();
  setupTime();
  nextEpochTime = getTime() + loopIntervalSec;
  Serial.println("---");
}

void loop()
{
  webSocket.loop(); // Keep the socket alive

  /**
   * Using a delay(3000) in the loop caused
   * the websocket connection to fail.
   * As a workaround, measure epoch on
   * each iteration, broadcast new
   * data when enough time passes.
   */
  epochTime = getTime();
  if (epochTime > nextEpochTime)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Epoch Time: " + String(epochTime));
    nextEpochTime = epochTime + loopIntervalSec;

    short cabHumidity = getHumidity();
    short cabTempF = getTemperature();
    int heaterPinVal = getPinState();

    StringSumHelper Json = "{";
    Json += "\"cabHumidity\":" + String(cabHumidity);
    Json += ",\"heaterPinVal\":" + String(heaterPinVal);
    Json += ",\"cabTempF\":" + String(cabTempF);
    Json += ",\"chipId\": \"" + chipId + "\"";
    Json += ",\"updatedAt\":" + String(epochTime);
    Json += "}";
    webSocket.sendTXT(Json);
    Serial.println(Json);
    Serial.println("---");
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
  }
}

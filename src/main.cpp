#include "wsHeater.h"
#include "wifiConnect.h"
#include <ArduinoJson.h>

// GPIO
const int thermoPin = 33; // Thermostat GPIO pin

void setup()
{
  Serial.begin(115200);
  wifiConnect();
  wsHeaterSetup();
  Serial.println("---");
}

void loop()
{
  wsHeaterLoop();
}

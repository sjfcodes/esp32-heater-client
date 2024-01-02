#include <ArduinoJson.h>
#include <WebServer.h>
#include <WebSocketsClient.h>
#include <WiFi.h>

const int heaterPin = 32;    // Heater GPIO pin

void heaterOn()
{
    digitalWrite(LED_BUILTIN, HIGH); // Led on
    digitalWrite(heaterPin, HIGH);   // Heat on
    Serial.println("Heater on");     // Print action log
}

void heaterOff()
{
    digitalWrite(LED_BUILTIN, LOW); // Led off
    digitalWrite(heaterPin, LOW);   // Heat off
    Serial.println("Heater off");   // Print action log
}

void heaterSetup()
{
    pinMode(heaterPin, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
}

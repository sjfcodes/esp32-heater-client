#include <ArduinoJson.h>
#include <WebServer.h>
#include <WebSocketsClient.h>
#include <WiFi.h>

const short heaterPin = 32; // Heater GPIO pin

void heaterOn()
{
    digitalWrite(heaterPin, HIGH);   // Heat on
    Serial.println("Heater on");     // Print action log
}

void heaterOff()
{
    digitalWrite(heaterPin, LOW);   // Heat off
    Serial.println("Heater off");   // Print action log
}

void heaterSetup()
{
    pinMode(heaterPin, OUTPUT);
}

short getPinState()
{
    return (short)digitalRead(heaterPin);
}
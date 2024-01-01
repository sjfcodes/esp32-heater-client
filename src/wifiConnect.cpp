#include <WiFi.h>

// Wifi Credentials
const char *ssid = ""; // Wifi SSID
const char *password = "";    // Wifi Password

void wifiConnect()
{
    WiFi.begin(ssid, password); // Connect to WiFi
    Serial.println("");
    Serial.print("WiFi Connecting");
    while (WiFi.status() != WL_CONNECTED) // Waiting on Wifi
    {
        Serial.print(".");
        delay(500);
    }

    Serial.println();                                      // Advance terminal line
    Serial.println("WiFi Connected!");                     //
    Serial.println("Local IP: " + String(WiFi.localIP())); // Print action log
}
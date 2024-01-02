#include "DHT.h"

#define DHTPIN 13 // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

void sensorSetup()
{
    dht.begin();
    Serial.println("Setting up DHT22.");
}
float getHumidity()
{
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Check if any reads failed and exit early (to try again).
    if (isnan(h))
    {
        Serial.println(F("Failed to read humidity from DHT sensor!"));
        return h;
    }

    return h;
}
void getTemperature()
{
}
void get()
{
}
void sensorLoop()
{
    // Wait a few seconds between measurements.

    // // Reading temperature or humidity takes about 250 milliseconds!
    // // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    // float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(t) || isnan(f))
    {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
    }

    Serial.print(F("Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(f);
    Serial.print(F("°F"));
}
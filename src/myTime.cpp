#include <ArduinoJson.h>
#include "time.h"

const char *ntpServer = "pool.ntp.org"; // NTP server to request epoch time

void setupTime()
{
    configTime(0, 0, ntpServer);
}
unsigned long getTime() // Function that gets current epoch time
{
    time_t now;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        // Serial.println("Failed to obtain time");
        return (0);
    }
    time(&now);
    return now;
}

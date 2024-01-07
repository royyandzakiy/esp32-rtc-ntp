#include <WiFi.h>
#include "time.h"
#include "esp_sntp.h"
#include "credentials.h"

// Replace with your network credentials (STATION)
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600 * 7;
const int   daylightOffset_sec = 3600;

void timeSyncCallback(timeval * tmv) {
  Serial.println("Success to obtain time from NTP, RTC updated!");
  
  // set a flag here if needed time synced
  // ...

  // turn off wifi for demonstration purposes
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

// print local time in a readable format
void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%d/%b/%Y %H:%M:%S");
}

// Variable to hold current epoch timestamp
unsigned long epochTime; 

// getEpochTime() Function that gets current epoch time. already in unix timestamp format
unsigned long getEpochTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

void setup()
{
  Serial.begin(115200);
  
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  sntp_set_time_sync_notification_cb(timeSyncCallback);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); // JUST THIS  
}

void loop()
{
  // printLocalTime();
  epochTime = getEpochTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);
  delay(1000);
}

#pragma once
#ifndef net_H
#define net_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiS3.h>

namespace Net {

const char WIFI_SSID[] = "edna";      // your network SSID (name)
const char WIFI_PASS[] = "ednamode";  // your network password (use for WPA, or use as key for WEP)
const int SERVER_PORT = 80;           // Server Port

extern int status;         // WIFI Status
extern WiFiServer server;  // Server

void startWiFi();                                  // Start WiFi using UNO R4 Access Point
void printWiFiStatus();                            // Print out WiFi Status
JsonDocument recieveBuggyData(WiFiClient client);  // Recieve data from Client Stream + Deserializes JSON

}

#endif
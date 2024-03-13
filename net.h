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

void startWiFi();
void printWiFiStatus();
JsonDocument recieveBuggyData(WiFiClient client);

}

#endif
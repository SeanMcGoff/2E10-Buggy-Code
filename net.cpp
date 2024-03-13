#include "WiFiClient.h"
#include "net.h"

int Net::status = WL_IDLE_STATUS;

WiFiServer Net::server(Net::SERVER_PORT);

void Net::startWiFi() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(Net::WIFI_SSID);

  // Create open network
  Net::status = WiFi.beginAP(Net::WIFI_SSID, Net::WIFI_PASS);
  // Fail Case of Creating Access Point
  if (Net::status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    while (true)
      ;
  }

  // wait for connection:
  delay(5000);

  // Start the web server
  Net::server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();
}

void Net::printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

JsonDocument Net::recieveBuggyData(WiFiClient client) {
  JsonDocument doc;
  if (client.connected()) {
    DeserializationError err = deserializeJson(doc, client);
    if (err) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(err.c_str());
    }
  }
  return doc;
}

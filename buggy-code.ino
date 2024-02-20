#include "motors.h"
#include <WiFiS3.h>
const char ssid[] = "edna";        // your network SSID (name)
const char pass[] = "ednamode";    // your network password (use for WPA, or use as key for WEP)
 

int status = WL_IDLE_STATUS; // WIFI Status
WiFiServer server(80); // Server

typedef enum RequestStatus {
  START,
  STOP,
  UNDEFINED
};

int led =  LED_BUILTIN;

// IR Defs
const int LEYE = D12;
const int REYE = D13;
bool irState[2];

bool motorsActivated = false; // Run Motor Code Flag

void updateBuggyMotors() {
  irState[0] = digitalRead(LEYE);
  irState[1] = digitalRead(REYE);
  if(irState[0] == HIGH && irState[1] == HIGH) {
    Serial.println("Here 1");
    // On the white path
    leftMotorForward(200);
    rightMotorForward(180);
  } else if(irState[0] == LOW && irState[1] == LOW) {
    Serial.println("Here 2");
    // Not on any path
    stopMotors();
  } else if(irState[0] == LOW && irState[1] == HIGH) {
    Serial.println("Here 3");
    // Turn Left
    rightMotorForward(180);
    leftMotorForward(0);
  } else {
    Serial.println("Here 4");
    // Turn Right 
    leftMotorForward(200);
    rightMotorForward(0);
  }
}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}


// Starts the wifi module
void startWiFi() {
  Serial.begin(9600);
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  // print the network name (SSID);
  Serial.println("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    while (true);
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();
}


RequestStatus recieveRequest(WiFiClient client) {
  Serial.println("new client");           // print a message out the Serial port
  String currentLine = "";                // make a String to hold incoming data from the client
  while (client.connected()) {
    delayMicroseconds(10);
    if(client.available()) {
      char c = client.read();
      Serial.print(c);
      if(c == '\n') {
        Serial.println("");
          if (currentLine.startsWith("GET /START")) {
            client.println("ACTIVATE THE MOTORS!!!");
            Serial.println("ACTIVATE THE MOTORS!!!");
            Serial.println("client disconnected");
            client.stop();
            return RequestStatus::START;
          } else if (currentLine.startsWith("GET /STOP")) {
            client.println("DEACTIVATE THE MOTORS!!!");
            Serial.println("DEACTIVATE THE MOTORS!!!");
            Serial.println("client disconnected");
            client.stop();
            return RequestStatus::STOP;
          } else {
            currentLine = ""; 
          }
          if(currentLine == 0) {
            client.println("YOU DID FUCK ALL WITH THE MOTORS!!!");
            Serial.println("YOU DID FUCK ALL WITH THE MOTORS!!!");
            Serial.println("client disconnected");
            client.stop();
            return RequestStatus::UNDEFINED;
          }
      }
      if(c != '\r') currentLine += c;
    }
  } 
  client.stop();
  return RequestStatus::UNDEFINED;
}

void setup() {
  Serial.begin(9600);
  
  pinMode(led, OUTPUT);      // set the LED pin mode
  // Set IR Sensor pin modes
  pinMode( LEYE, INPUT );
  pinMode( REYE, INPUT );

  // Set PWM Motor Speed
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);
  
  // Direction Output
  pinMode(LEFT_MOTOR_DIRECTION[0], OUTPUT);
  pinMode(LEFT_MOTOR_DIRECTION[1], OUTPUT);
  pinMode(RIGHT_MOTOR_DIRECTION[0], OUTPUT);
  pinMode(RIGHT_MOTOR_DIRECTION[1], OUTPUT);

  startWiFi(); // Starts the wifi
}

void loop() {

  WiFiClient client = server.available();   // listen for incoming clients
  if(client) {
    RequestStatus status = recieveRequest(client);
    switch (status) {
    case RequestStatus::START:
      motorsActivated = true;
      break;
    case RequestStatus::STOP:
      motorsActivated = false;
      break;
    case RequestStatus::UNDEFINED:
      break;
    }
  }
  if(motorsActivated) updateBuggyMotors();
  else stopMotors();
  delay(50);
} // loop()

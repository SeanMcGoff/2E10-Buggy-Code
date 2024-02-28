#include "motors.h"
#include <ArduinoJson.h>
#include <WiFiS3.h>
const char ssid[] = "edna";      // your network SSID (name)
const char pass[] = "ednamode";  // your network password (use for WPA, or use as key for WEP)


int status = WL_IDLE_STATUS;  // WIFI Status

WiFiServer server(80);  // Server

typedef enum RequestStatus {
  START,
  STOP,
  UNDEFINED
};

int led = LED_BUILTIN;

// IR Defs
const int LEYE = D12;
const int REYE = D13;
bool irState[2];

// US Sensor Defs
const int US_ECHO = 2;
const int US_TRIG = 3;
const int DISTANCE_SENS = 15;
bool obstacle_detected = false;

bool motorsActivated = false;  // Run Motor Code Flag

// Wheel Encoder Defs
const int LEFT_ENCODER = D4;
const int RIGHT_ENCODER = D7;
const double WHEEL_CIRCUMFRENCE = 6.3 * PI;
bool encoderState[2] = {true, true};
double wheelDistance = 0.0;

unsigned long startTimer = millis(); // Timer Thing

void updateBuggyMotors() {
  irState[0] = digitalRead(LEYE);
  irState[1] = digitalRead(REYE);
  if (irState[0] == HIGH && irState[1] == HIGH) {
    // On the white path
    leftMotorForward(200);
    rightMotorForward(180);
  } else if (irState[0] == LOW && irState[1] == LOW) {
    // Not on any path
    stopMotors();
  } else if (irState[0] == LOW && irState[1] == HIGH) {
    // Turn Left
    rightMotorForward(180);
    leftMotorForward(0);
  } else {
    // Turn Right
    leftMotorForward(200);
    rightMotorForward(0);
  }
}

void updateEncoders() {
  bool leftState = digitalRead(LEFT_ENCODER);
  bool rightState = digitalRead(LEFT_ENCODER);
  if(leftState != encoderState[0]) {
    encoderState[0] = leftState;
    wheelDistance += WHEEL_CIRCUMFRENCE / 8;
  }
  if(rightState != encoderState[1]) {
    encoderState[1] = rightState;
    wheelDistance += WHEEL_CIRCUMFRENCE / 8;
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
}


// Starts the wifi module
void startWiFi() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    while (true)
      ;
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();
}

int getUSDistance() {
  long duration;
  digitalWrite(US_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(US_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_TRIG, LOW);

  duration = pulseIn(US_ECHO, HIGH);

  return duration / 58;
}

RequestStatus recieveRequest(WiFiClient client) {
  Serial.print("Recieving Request: ");  // print a message out the Serial port
  String currentLine = "";       // make a String to hold incoming data from the client
  if(client.connected()) {
    delayMicroseconds(10);
    while(client.available()) {
      char c = client.read();
      //Serial.print(c);
      if (c == '\n') {
        // START CASE
        if(currentLine == "start") {
          Serial.println("Start Motors");
          return RequestStatus::START;
        }
        // STOP CASE
        if(currentLine == "stop") {
          Serial.println("Stop Motors");
          return RequestStatus::STOP;
        }
        // PING CASE
        if (currentLine == "ping") {
          Serial.println("Ping");
          JsonDocument doc;
          doc["obstacle_detected"] = obstacle_detected;
          doc["distance_traveled"] = wheelDistance;
          serializeJsonPretty(doc,client);
          return RequestStatus::UNDEFINED;
        }
        // BAD COMMAND CASE
        Serial.println(" Bad Command");
        return RequestStatus::UNDEFINED;
        }
      if (c != '\r') currentLine += c;
    }
  }
  // In the weird case it gets heres
  return RequestStatus::UNDEFINED;
}

void setup() {
  Serial.begin(9600);

  pinMode(led, OUTPUT);  // set the LED pin mode
  // Set IR Sensor pin modes
  pinMode(LEYE, INPUT);
  pinMode(REYE, INPUT);

  // Set PWM Motor Speed
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);

  // Direction Output
  pinMode(LEFT_MOTOR_DIRECTION[0], OUTPUT);
  pinMode(LEFT_MOTOR_DIRECTION[1], OUTPUT);
  pinMode(RIGHT_MOTOR_DIRECTION[0], OUTPUT);
  pinMode(RIGHT_MOTOR_DIRECTION[1], OUTPUT);

  // Ultrasonic Sensor Modes
  pinMode(US_TRIG, OUTPUT);
  pinMode(US_ECHO, INPUT);

  // Wheel Encoder Modes
  pinMode(LEFT_ENCODER,INPUT_PULLUP);
  pinMode(RIGHT_ENCODER,INPUT_PULLUP);

  startWiFi();  // Starts the wifi
}

void loop() {
  if(millis() - startTimer >= 250) {
    WiFiClient client = server.available();  // listen for incoming clients
    if (client && client.available()) {
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
    startTimer = millis();
  }

  updateEncoders();

  Serial.print("Wheel Distance: ");
  Serial.print(wheelDistance);
  Serial.println(" cm.");

  int distance = getUSDistance();
  if (distance > DISTANCE_SENS) {
    obstacle_detected = false;
    if (motorsActivated) {
      updateBuggyMotors();
    }
    else stopMotors();
  } else {
    obstacle_detected = true;
    stopMotors();
  }
  //Serial.print("Distance detected: ");
  //Serial.print(distance);
  //Serial.println(" cm");
  delay(10);
}  // loop()

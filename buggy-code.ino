#include "ir.h"
#include "motors.h"
#include "net.h"
#include "pid.h"
#include "ultrasonic.h"
#include "wheel-encoders.h"

#include <ArduinoJson.h>

bool obstacle_detected = false;             // Obstacle Detected Flag
unsigned long US_Trigger_Timer = millis();  // Loop For US Sensor Processing
unsigned long Web_Timer = millis();         // Timer for Web Processing

int speed = 255;         // Speed (as defined by PID)
double speed_coeff = 1;  // Speed coefficient (defined by GUI)


// Updates the Buggy's Motors Given IR State
void updateBuggyMotors() {
  IR::updateState();
  if (IR::state[0] == HIGH && IR::state[1] == HIGH) {
    // On the white path
    Motors::leftForward(ceil(speed * speed_coeff));
    Motors::rightForward(ceil(speed * Motors::CORRECTION * speed_coeff));
  } else if (IR::state[0] == LOW && IR::state[1] == LOW) {
    // Not on any path
    Motors::bothStop();
  } else if (IR::state[0] == LOW && IR::state[1] == HIGH) {
    // Turn Left
    Motors::rightForward(ceil(speed * Motors::CORRECTION * speed_coeff));
    Motors::leftForward(0);
  } else {
    // Turn Right
    Motors::leftForward(ceil(speed * speed_coeff));
    Motors::rightBackward(ceil(speed * speed_coeff * 0.1));
  }
}

// Parse JSON from GUI and preform relevant function
void parseGUIRequest(JsonDocument doc, WiFiClient client) {
  String command = doc["command"];

  // Start Command
  if (command == "start") {
    Serial.println("Start Motors");
    Motors::activated = true;
  }

  // Stop Command
  else if (command == "stop") {
    Serial.println("Stop Motors");
    Motors::activated = false;
  }

  // Ping Command
  else if (command == "ping") {
    JsonDocument response_doc;
    response_doc["obstacle_detected"] = obstacle_detected;
    response_doc["distance_traveled"] = WheelEncoders::distanceTraveled;
    response_doc["speed"] = speed;
    serializeJsonPretty(response_doc, client);
  }

  // Set Speed command
  else if (command == "setSpeed") {
    double clientSpeed = doc["speed"];
    Serial.print("Setting speed to: ");
    Serial.println(clientSpeed);
    speed_coeff = clientSpeed;
  }

  // Incorrect / Empty Command
  else {
    Serial.println("Error: Bad Command");
  }
}

void setup() {
  // Begin Serial Communication
  Serial.begin(9600);

  // Set H-Bridge Pin Modes
  pinMode(Motors::LEFT_HBRIDGE[0], OUTPUT);
  pinMode(Motors::LEFT_HBRIDGE[1], OUTPUT);
  pinMode(Motors::RIGHT_HBRIDGE[0], OUTPUT);
  pinMode(Motors::RIGHT_HBRIDGE[1], OUTPUT);

  // Set IR Sensor Pin Modes
  pinMode(IR::LEYE, INPUT);
  pinMode(IR::LEYE, INPUT);

  // Set PWM Pin Modes
  pinMode(Motors::LEFT_PWM, OUTPUT);
  pinMode(Motors::LEFT_PWM, OUTPUT);

  // Set US Pin Modes
  pinMode(US::TRIG, OUTPUT);
  pinMode(US::ECHO, INPUT_PULLUP);

  // Set US Interrupts
  attachInterrupt(digitalPinToInterrupt(US::ECHO), US::Echo_Rising, RISING);
  attachInterrupt(digitalPinToInterrupt(US::ECHO), US::Echo_Falling, FALLING);

  // Set Wheel Encoder Pin Modes
  pinMode(WheelEncoders::LEFT, INPUT_PULLUP);
  pinMode(WheelEncoders::RIGHT, INPUT_PULLUP);

  Net::startWiFi();  // Start the WiFi
}  // setup()

void loop() {

  // Check for Client Request 500 Milliseconds
  if (millis() - Web_Timer >= 500) {
    WiFiClient client = Net::server.available();  // listen for incoming clients
    if (client && client.available()) {
      JsonDocument doc = Net::recieveBuggyData(client);
      parseGUIRequest(doc, client);
    }
    Web_Timer = millis();
  }

  // Trigger US Sensor Every 250 Milliseconds
  if (millis() - US_Trigger_Timer >= 250) {
    US::Trigger();
    US_Trigger_Timer = millis();
  }

  obstacle_detected = US::getCurrentDistance() <= US::DISTANCE_SENS;  // Update Obstacle Detection

  WheelEncoders::update();  // Update the Wheel Encoders

  speed = PID::computePID(US::getCurrentDistance());  // Update Speed with PID

  // Update Buggy Motors
  if (Motors::activated) updateBuggyMotors();
  // If motors are deactivated, stop motors and reset distance traveled
  else {
    Motors::bothStop();
    WheelEncoders::distanceTraveled = 0;
  }

}  // loop()

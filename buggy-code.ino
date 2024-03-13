#include "ir.h"
#include "motors.h"
#include "net.h"
#include "ultrasonic.h"
#include "wheel-encoders.h"

#include <ArduinoJson.h>

bool obstacle_detected = false; // Obstacle Detected Flag
unsigned long US_Trigger_Timer = millis();  // Loop For US Sensor Triggering

bool motorsActivated = false;  // Run Motor Code Flag

//PID constants
double kp = 30;
double ki = 0;
double kd = 50;

unsigned long currentTime = 0;
unsigned long previousTime = 0;
double elapsedTime;
double error;
double lastError = 0;
const double SetPoint = 15;
double cumError, rateError;

// ino-side Motor Vars
const double MOTOR_CORRECTION = 0.85;
int speed = 255;

unsigned long Web_Timer = millis();  // Timer for Web

void updateBuggyMotors() {
  IR::updateState();
  if (IR::state[0] == HIGH && IR::state[1] == HIGH) {
    // On the white path
    Motors::leftForward(speed);
    Motors::rightForward(ceil(speed * MOTOR_CORRECTION));
  } else if (IR::state[0] == LOW && IR::state[1] == LOW) {
    // Not on any path
    Motors::bothStop();
  } else if (IR::state[0] == LOW && IR::state[1] == HIGH) {
    // Turn Left
    Motors::rightForward(ceil(speed * MOTOR_CORRECTION));
    Motors::leftForward(0);
  } else {
    // Turn Right
    Motors::leftForward(speed);
    Motors::rightBackward(25);
  }
}

void parseRequest(JsonDocument doc, WiFiClient client) {
  String command = doc["command"];
  // Start Command
  if (command == "start") {
    Serial.println("Start Motors");
    motorsActivated = true;
  }
  // Stop Command
  else if (command == "stop") {
    Serial.println("Stop Motors");
    motorsActivated = false;
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
    speed = clientSpeed;
  }
  // If Incorrect / Empty Command
  else {
    Serial.println("Error: Bad Command");
  }
}

void setup() {
  Serial.begin(9600);

  // Set IR Sensor Pin Modes
  pinMode(IR::LEYE, INPUT);
  pinMode(IR::LEYE, INPUT);

  // Set PWM Pin Modes
  pinMode(Motors::LEFT_PWM, OUTPUT);
  pinMode(Motors::LEFT_PWM, OUTPUT);

  // Set H-Bridge Pin Modes
  pinMode(Motors::LEFT_HBRIDGE[0], OUTPUT);
  pinMode(Motors::LEFT_HBRIDGE[1], OUTPUT);
  pinMode(Motors::RIGHT_HBRIDGE[0], OUTPUT);
  pinMode(Motors::RIGHT_HBRIDGE[1], OUTPUT);

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
}

int computePID(long distance) {
  currentTime = millis();                              //get current time
  elapsedTime = (double)(currentTime - previousTime);  //compute time elapsed from previous computation

  error = (US::getCurrentDistance() - SetPoint);  // determine error
  cumError += error * elapsedTime;                // compute integral
  rateError = (error - lastError) / elapsedTime;  // compute derivative

  int out = kp * error + ki * cumError + kd * rateError;  //PID output

  out = constrain(out, 0, 255);

  lastError = error;           //remember current error
  previousTime = currentTime;  //remember current time

  return out;  //have function return the PID output
}

void loop() {

  if (millis() - Web_Timer >= 500) {
    WiFiClient client = Net::server.available();  // listen for incoming clients
    if (client && client.available()) {
      JsonDocument doc = Net::recieveBuggyData(client);
      parseRequest(doc, client);
    }
    Web_Timer = millis();
  }

  WheelEncoders::update(); // Update the Wheel Encoders

  obstacle_detected = US::getCurrentDistance() <= US::DISTANCE_SENS;

  if (millis() - US_Trigger_Timer >= 250) {
    US::Trigger();
    US_Trigger_Timer = millis();
  }

  speed = computePID(US::getCurrentDistance());

  if (motorsActivated) {
    updateBuggyMotors();
  } else {
    Motors::bothStop();
    WheelEncoders::distanceTraveled = 0;
  }

}  // loop()

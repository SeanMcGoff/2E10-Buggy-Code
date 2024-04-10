#include "src/camera.h"
#include "src/chacha.h"
#include "src/ir.h"
#include "src/ledmatrix.h"
#include "src/motors.h"
#include "src/net.h"
#include "src/pid.h"
#include "src/ultrasonic.h"
#include "src/wheel-encoders.h"

#include <ArduinoJson.h>

bool obstacle_detected = false;  // Obstacle Detected Flag
unsigned long Husky_Timer = millis();
unsigned long US_Trigger_Timer = millis();     // Loop For US Sensor Processing
unsigned long Web_Timer = millis();            // Timer for Web Processing
unsigned long Wheel_Encoder_Timer = millis();  // Timer for Wheel Encoder
unsigned long Cha_Cha_Timer = millis();

double speed_coeff = 1;  // Speed coefficient (defined by GUI)
bool manual_override = false;


// Buggy update code if buggy is not at a junction
void noJunctionUpdateBuggy(int lMotor, int rMotor) {
  if (IR::state[0] == HIGH && IR::state[1] == HIGH) {
    // On the white path
    Motors::leftForward(lMotor);
    Motors::rightForward(rMotor);
  } else if (IR::state[0] == LOW && IR::state[1] == LOW) {
    // Both are on white (at junction)
    Motors::onJunction = true;
  } else if (IR::state[0] == LOW && IR::state[1] == HIGH) {
    // Turn Left
    Motors::rightForward(lMotor);
    Motors::leftStop();
  } else {
    // Turn Right
    Motors::leftForward(lMotor);
    Motors::rightStop();
  }
}

// Buggy update Code if buggy is at a junction
void junctionUpdateBuggy(int lMotor, int rMotor) {
  // Turn left or right given most recent direction-related aprilTag
  switch (Camera::junctionTurnDirection) {
    case Camera::TAG::LEFT:
      Motors::rightForward(lMotor);
      Motors::leftBackward(rMotor);
      break;
    case Camera::TAG::RIGHT:
      Motors::leftForward(lMotor);
      Motors::rightBackward(rMotor);
      break;
  }
}

// Updates the Buggy's Motors Given IR State
void updateBuggyMotors(bool isPid) {
  IR::updateState();

  int lMotor, rMotor;
  // Determine Motor Speeds
  if (isPid) {
    lMotor = Pid::Output;
    rMotor = ceil(Pid::Output * Motors::CORRECTION);
  } else if (Camera::isSpeedLimited) {
    lMotor = ceil(200 * Camera::SPEED_LIMIT_COEFF);
    rMotor = ceil(200 * Motors::CORRECTION * Camera::SPEED_LIMIT_COEFF);
  } else {
    lMotor = ceil(200 * speed_coeff);
    rMotor = ceil(200 * Motors::CORRECTION * speed_coeff);
  }

  // If in junction mode and not on junction anymore, resume to normal function
  if (Motors::onJunction && IR::state[0] == HIGH && IR::state[1] == HIGH) {
    Motors::onJunction = false;
  }
  // Update Buggy Motors given junction state
  if (Motors::onJunction) junctionUpdateBuggy(lMotor, rMotor);
  else noJunctionUpdateBuggy(lMotor, rMotor);
}

// Parse JSON from GUI and preform relevant function
void parseGUIRequest(JsonDocument doc, WiFiClient client) {

  String command = doc["command"];

  // Start Command
  if (command == "start") {
    //Serial.println("Start Motors");
    Motors::activated = true;
  }

  // Stop Command
  else if (command == "stop") {
    //Serial.println("Stop Motors");
    Motors::activated = false;
  }

  // Ping Command
  else if (command == "ping") {
    JsonDocument response_doc;
    response_doc["control_strategy"] = (!manual_override && Pid::enabled) ? 2 : 1;
    response_doc["distance_from_object"] = US::getCurrentDistance();
    response_doc["distance_traveled"] = WheelEncoders::distanceTraveled;
    response_doc["measured_speed"] = WheelEncoders::velocity;
    response_doc["obstacle_detected"] = obstacle_detected;
    response_doc["tag_recognised"] = static_cast<int>(Camera::mostRecentTag);

    String o;
    serializeJson(response_doc, o);
    client.println(o);
  }

  // Set Speed command
  else if (command == "setSpeed") {
    float clientSpeed = doc["speed"];
    speed_coeff = clientSpeed;
  }

  // Set PID Command
  else if (command = "setManualOverride") {
    bool o = doc["enabled"];
    manual_override = o;
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
  pinMode(IR::REYE, INPUT);

  // Set PID Mode
  Pid::buggyPID.SetMode(AUTOMATIC);
  Pid::buggyPID.SetOutputLimits(0, 255);

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

  LED_Matrix::start();  // Start the LED Matrix
  Camera::start();      // Start the Camera
  Net::startWiFi();     // Start the WiFi
}  // setup()

void controlStrategy1() {
  if (!obstacle_detected && Motors::activated) {
    // Update Buggy Motors
    updateBuggyMotors(false);
  } else {
    // If motors are deactivated or obstacle detected, stop motors and reset distance traveled
    Motors::bothStop();
    WheelEncoders::distanceTraveled = 0;
  }
}

void controlStrategy2() {
  // Update Buggy Motors
  if (Motors::activated) updateBuggyMotors(true);
  // If motors are deactivated, stop motors and reset distance traveled
  else {
    Motors::bothStop();
    WheelEncoders::distanceTraveled = 0;
  }
}

void slideThatShit() {
  if (ChaCha::firstEnable) {
    Cha_Cha_Timer = millis();
    ChaCha::beatNum = 0;
    ChaCha::firstEnable = false;
  }
  if (millis() - Cha_Cha_Timer >= ChaCha::MS_PER_BEAT) {
    Cha_Cha_Timer = millis();
    ChaCha::beatNum++;

    // Escape at end of song
    if (ChaCha::beatNum >= ChaCha::SLIDE_SEQUENCE_SIZE) {
      Camera::slideMode = false;
      ChaCha::firstEnable = true;
      return;
    }

    Serial.println(ChaCha::beatNum);
    Serial.println(ChaCha::SLIDE_SEQUENCE[ChaCha::beatNum]);
    // The actual commands
    switch (ChaCha::SLIDE_SEQUENCE[ChaCha::beatNum]) {
      case ChaCha::COMMANDS::NOTHING:
        Motors::bothStop();
        LED_Matrix::clear();
        break;
      case ChaCha::COMMANDS::FLASH:
        LED_Matrix::flash();
        break;
      case ChaCha::COMMANDS::CLAPON:
        LED_Matrix::clap();
        break;
      case ChaCha::COMMANDS::CLAPOFF:
        LED_Matrix::clear();
        break;
      case ChaCha::COMMANDS::FORWARD:
        Motors::leftForward(255);
        Motors::rightForward(255);
        break;
      case ChaCha::COMMANDS::BACK:
        Motors::leftBackward(255);
        Motors::rightBackward(255);
        break;
      case ChaCha::COMMANDS::LEFT:
        Motors::leftStop();
        Motors::rightForward(255);
        break;
      case ChaCha::COMMANDS::RIGHT:
        Motors::leftForward(255);
        Motors::rightStop();
        break;
      case ChaCha::COMMANDS::CHACHALEFT:
        Motors::leftBackward(100);
        Motors::rightForward(200);
        break;
      case ChaCha::COMMANDS::CHACHARIGHT:
        Motors::leftForward(200);
        Motors::rightBackward(100);
        break;
    }
  }
}

void loop() {
  // Check for Client Request 250 Milliseconds
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

  if (millis() - Husky_Timer >= 250) {
    Camera::updateMostRecentTag();
    switch (Camera::mostRecentTag) {
      case Camera::TAG::LEFT:
        LED_Matrix::left();
        break;
      case Camera::TAG::RIGHT:
        LED_Matrix::right();
        break;
      case Camera::TAG::SLOW:
        LED_Matrix::slow();
        break;
      case Camera::TAG::MAXSPEED:
        LED_Matrix::fast();
        break;
    }
  }

  double currentUSDistance = US::getCurrentDistance();

  Pid::enabled = currentUSDistance <= Pid::ENABLING_DISTANCE;
  Pid::Input = currentUSDistance - Pid::SetPoint;  // Calculate new PID Input
  Pid::buggyPID.Compute();                         // Compute new Output

  obstacle_detected = currentUSDistance <= Pid::SetPoint;  // Update Obstacle Detection

  WheelEncoders::update();                                                                                                       // Update the Wheel Encoders
  if (!Motors::activated || obstacle_detected || ((IR::state[0] == LOW) && (IR::state[1] == LOW))) WheelEncoders::velocity = 0;  // I hate this line so much


  // The cha cha trumps all
  if (Camera::slideMode) {
    slideThatShit();
  } else if (!manual_override && Pid::enabled) {
    controlStrategy2();
  } else {
    controlStrategy1();
  }
}  // loop()

#include "motors.h"

void Motors::leftForward(int speed) {
  digitalWrite(LEFT_HBRIDGE[0], HIGH);
  digitalWrite(LEFT_HBRIDGE[1], LOW);
  analogWrite(LEFT_PWM, speed);
}

void Motors::leftBackward(int speed) {
  digitalWrite(LEFT_HBRIDGE[0], LOW);
  digitalWrite(LEFT_HBRIDGE[1], HIGH);
  analogWrite(LEFT_PWM, speed);
}

void Motors::rightForward(int speed) {
  digitalWrite(RIGHT_HBRIDGE[0], HIGH);
  digitalWrite(RIGHT_HBRIDGE[1], LOW);
  analogWrite(RIGHT_PWM, speed);
}

void Motors::rightBackward(int speed) {
  digitalWrite(RIGHT_HBRIDGE[0], LOW);
  digitalWrite(RIGHT_HBRIDGE[1], HIGH);
  analogWrite(RIGHT_PWM, speed);
}

void Motors::leftStop() {
  digitalWrite(LEFT_HBRIDGE[0], LOW);
  digitalWrite(LEFT_HBRIDGE[1], LOW);
}

void Motors::rightStop() {
  digitalWrite(RIGHT_HBRIDGE[0], LOW);
  digitalWrite(RIGHT_HBRIDGE[1], LOW);
}

void Motors::bothStop() {
  Motors::leftStop();
  Motors::rightStop();
}
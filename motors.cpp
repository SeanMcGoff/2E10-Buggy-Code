#include "motors.h"

void leftMotorForward(int speed) {
  digitalWrite(LEFT_MOTOR_DIRECTION[0],HIGH);
  digitalWrite(LEFT_MOTOR_DIRECTION[1],LOW);
  analogWrite(LEFT_MOTOR_PWM,speed);
}

void leftMotorBackward(int speed) {
  digitalWrite(LEFT_MOTOR_DIRECTION[0],LOW);
  digitalWrite(LEFT_MOTOR_DIRECTION[1],HIGH);
  analogWrite(LEFT_MOTOR_PWM,speed);
}

void rightMotorForward(int speed) {
  digitalWrite(RIGHT_MOTOR_DIRECTION[0],HIGH);
  digitalWrite(RIGHT_MOTOR_DIRECTION[1],LOW);
  analogWrite(RIGHT_MOTOR_PWM,speed);
}

void rightMotorBackward(int speed) {
  digitalWrite(RIGHT_MOTOR_DIRECTION[0],LOW);
  digitalWrite(RIGHT_MOTOR_DIRECTION[1],HIGH);
  analogWrite(RIGHT_MOTOR_PWM,speed);
}

void leftMotorStop() {
  digitalWrite(LEFT_MOTOR_DIRECTION[0],LOW);
  digitalWrite(LEFT_MOTOR_DIRECTION[1],LOW); 
}

void rightMotorStop() {
  digitalWrite(RIGHT_MOTOR_DIRECTION[0],LOW);
  digitalWrite(RIGHT_MOTOR_DIRECTION[1],LOW); 
}

void stopMotors() {
  leftMotorStop();
  rightMotorStop();
}
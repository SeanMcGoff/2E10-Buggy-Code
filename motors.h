#ifndef MOTORS_H
#define MOTORS_H
#include <Arduino.h>
const int LEFT_MOTOR_PWM = D6;
const int RIGHT_MOTOR_PWM = D5;

// D8 and D9 Control Left Motor
const int LEFT_MOTOR_DIRECTION[2] = {D9,D8};

// D10 and D11 Control Right Motor
const int RIGHT_MOTOR_DIRECTION[2] = {D10,D11};


// Motor Function Defs
void leftMotorForward(int speed);
void leftMotorBackward(int speed);
void rightMotorForward(int speed);
void rightMotorBackward(int speed);
void leftMotorStop();
void rightMotorStop();
void stopMotors();

#endif
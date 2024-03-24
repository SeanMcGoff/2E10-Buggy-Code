#pragma once
#ifndef motors_H
#define motors_H
#include <Arduino.h>
namespace Motors {

const int LEFT_PWM = D6;                    // Left Motor PWM Pin
const int RIGHT_PWM = D5;                   // Right Motor PWM Pin
const int LEFT_HBRIDGE[2] = { D9, D8 };     // Pins that Control Left Motor H-Bridge
const int RIGHT_HBRIDGE[2] = { D10, D11 };  // Pins that Control Right Motor H-Bridge

const double CORRECTION = 0.6;  // Correction Factor for Left/Right Motors

extern bool activated;  // On-Off Flag for Motors (Toggled by GUI)

void leftForward(int speed);    // Moves Left Motor Forward
void leftBackward(int speed);   // Moves Left Motor Backward
void rightForward(int speed);   // Moves Right Motor Forward
void rightBackward(int speed);  // Moves Right Motor Backward
void leftStop();                // Stops Left Motor
void rightStop();               // Stops Right Motor
void bothStop();                // Stops Both Motors

}

#endif
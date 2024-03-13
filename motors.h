#pragma once
#ifndef motors_H
#define motors_H
#include <Arduino.h>
namespace Motors {

const int LEFT_PWM = D6;                    // Left Motor PWM Pin
const int RIGHT_PWM = D5;                   // Right Motor PWM Pin
const int LEFT_HBRIDGE[2] = { D9, D8 };     // Pins that Control Left Motor H-Bridge
const int RIGHT_HBRIDGE[2] = { D10, D11 };  // Pins that Control Right Motor H-Bridge

void leftForward(int speed);
void leftBackward(int speed);
void rightForward(int speed);
void rightBackward(int speed);
void leftStop();
void rightStop();
void bothStop();

}

#endif
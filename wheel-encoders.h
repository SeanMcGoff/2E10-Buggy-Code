#pragma once
#ifndef wheel_encoders_H
#define wheel_encoders_H
#include <Arduino.h>

namespace WheelEncoders {

const int LEFT = D4; // Left Wheel Encoder Pin
const int RIGHT = D7; // Right Wheel Encoder Pin
const double CIRCUMFRENCE = 6.3 * PI; // Circumfrence of Buggy Wheels (6.3π)

extern bool state[2]; // Last Recorded State of Wheel Encoders
extern double distanceTraveled; // Last Update of Distance Traveled

void update(); // Updates state and distanceTraveled

}

#endif